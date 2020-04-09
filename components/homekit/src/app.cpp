#include "app.hpp"

#include <esp_log.h>

#include "host_info.hpp"
#include "hap_defs.hpp"


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
