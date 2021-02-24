#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <sodium.h>
#include <lwip/err.h>
#include <lwip/sys.h>

#include "sdkconfig.h"

#include "rollkit.hpp"
#include "config.hpp"

using namespace std;

rollkit::App app;
rollkit::Accessory acc;
rollkit::Service acc_switch;
rollkit::Characteristic acc_switch_on_char;
rollkit::Characteristic acc_switch_name_char;

bool switch_on = false;

std::string get_mac_address() {
  string ret(17, 0);
  uint8_t mac[6];
  esp_wifi_get_mac(ESP_IF_WIFI_STA, mac);
  sprintf(&ret[0], "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return ret;
}

void init_app() {
  app.init(ACC_NAME, ACC_MODEL, ACC_MANUFACTURER, ACC_FIRMWARE_REVISION, ACC_SETUP_CODE, get_mac_address());

  acc_switch_on_char = rollkit::Characteristic(
    APPL_CHAR_UUID_ON,
    [](nlohmann::json v){ ESP_LOGD("acc", "Switch On Write: %d", v.get<int>()); switch_on = (bool)v.get<int>(); },
    []() -> nlohmann::json { ESP_LOGD("acc", "Switch On Read: %d", switch_on); return switch_on; },
    "bool",
    {"pr", "pw", "ev"}
  );
  acc_switch_name_char = rollkit::Characteristic(
    APPL_CHAR_UUID_NAME,
    [](nlohmann::json v){},
    []() -> nlohmann::json { ESP_LOGD("acc", "Switch Name Read"); return "Pump"; },
    "string",
    {"pr"}
  );
  acc_switch = rollkit::Service(
    APPL_SRVC_UUID_SWITCH,
    false,
    true
  );
  acc_switch.register_characteristic(acc_switch_on_char);
  acc_switch.register_characteristic(acc_switch_name_char);
  acc.register_service(acc_switch);

  app.register_accessory(acc);
  app.start();
}


// ESP32 WiFi handler.
static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    wifi_event_sta_disconnected_t* data = (wifi_event_sta_disconnected_t*)event_data;
    esp_wifi_connect();
    ESP_LOGI("main", "Disconnected(%d): retyring connection to WiFi network...", data->reason);
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
    ESP_LOGI("main", "Connected: IP: " IPSTR, IP2STR(&event->ip_info.ip));
    init_app();
  }
}


void init_wifi() {
  ESP_ERROR_CHECK(esp_netif_init());

  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

  wifi_config_t wifi_config;
  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.sta.ssid, WIFI_SSID);
  strcpy((char*)wifi_config.sta.password, WIFI_PASS);
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI("main", "WiFi Init finished.");
}


extern "C" {
	void app_main(void);
}
void app_main(void) {
  if(sodium_init() == -1){
    ESP_LOGE("Crypto", "libsodium failed ot initilize");
    esp_restart();
  } else  {
    ESP_LOGD("Crypto", "libsodium initialized");
  }
  nvs_flash_init();
  init_wifi();
}
