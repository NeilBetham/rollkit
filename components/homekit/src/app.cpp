#include "app.hpp"

#include <esp_log.h>

#include "host_info.hpp"
#include "hap_defs.hpp"

namespace {


// Pass HTTP requests off to App code
void mongoose_event_handler(struct mg_connection *nc, int event, void *event_data) {
  App* app = (App*)nc->mgr->user_data;
  app->handle_mg_event(nc, event, event_data);
}

// Task function to run mongoose
void main_task(void* data) {
  ESP_LOGD("app", "Main task starting");
  App* app = (App*)data;
  while (1) {
    app->run();
  }
}


} // namespace

void App::init(std::string name, std::string model, std::string manu, std::string firmware_rev) {
  _mdns_mgr = MDNSManager(name, model);

  // Setup accessory info service
  Service accessory_info(
    APPL_SRVC_UUID_ACCESSORY_INFO,
    false,
    false
  );

  // Firmware Rev
  accessory_info.register_characteristic({
    APPL_CHAR_UUID_FIRMWARE_REV,
    [](std::string s){},
    []{ return "1.0.0"; },
    "string",
    {"pr"}
  });

  // Identify
  accessory_info.register_characteristic({
    APPL_CHAR_UUID_IDENTIFY,
    [](std::string s){ ESP_LOGI("app", "Identify!"); },
    []{ return ""; },
    "bool",
    {"pw"}
  });

  // Manufacturer
  accessory_info.register_characteristic({
    APPL_CHAR_UUID_MANUFACTURER,
    [](std::string s){},
    [=]{ return manu; },
    "string",
    {"pr"}
  });

  // Model
  accessory_info.register_characteristic({
    APPL_CHAR_UUID_MODEL,
    [](std::string s){},
    [=]{ return model; },
    "string",
    {"pr"}
  });

  // Name
  accessory_info.register_characteristic({
    APPL_CHAR_UUID_NAME,
    [](std::string s){},
    [=]{ return name; },
    "string",
    {"pr"}
  });

  // Serial
  accessory_info.register_characteristic({
    APPL_CHAR_UUID_SERIAL_NUMBER,
    [](std::string s){},
    []{ return get_mac_address(); },
    "string",
    {"pr"}
  });

  Service protocol_info(
    APPL_SRVC_UUID_PROTOCOL_INFO,
    false,
    false
  );

  // Protcol Version
  protocol_info.register_characteristic({
    APPL_CHAR_UUID_VERSION,
    [](std::string s){},
    []{ return "1.1.0"; },
    "string",
    {"pr"}
  });

  _accessory_info = accessory_info;
  _protocol_info = protocol_info;
}

void App::start() {
  ESP_LOGD("app", "RollKit Task starting");
  mg_mgr_init(&_mg_mgr, this);

  auto conn = mg_bind(&_mg_mgr, ":80", mongoose_event_handler);
  if(!conn) {
    ESP_LOGE("app", "Mongoose failed to bind to port");
    return;
  }
  ESP_LOGD("app", "Successfully bound");

  mg_set_protocol_http_websocket(conn);
  xTaskCreatePinnedToCore(&main_task, "rollkit_main", 20000, this, 5, &_main_task, 0);
}

void App::stop() {
  vTaskDelete(_main_task);
}


void App::run() {
  mg_mgr_poll(&_mg_mgr, 1000);
}
