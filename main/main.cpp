#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <nvs_flash.h>
#include <sodium.h>
#include <lwip/err.h>
#include <lwip/sys.h>

#include "config.hpp"

#include "mongoose.h"
#include "sdkconfig.h"

#include "mdns.hpp"
#include "session_manager.hpp"

static char l_tag []="http";
SessionManager sm;


// Convert a Mongoose string type to a string.
char *mg_str_to_str(struct mg_str mgStr) {
  char *retStr = (char *) malloc(mgStr.len + 1);
  memcpy(retStr, mgStr.p, mgStr.len);
  retStr[mgStr.len] = 0;
  return retStr;
}

// Simple Example HTTP event handler
void mongoose_event_handler(struct mg_connection *nc, int event, void *event_data) {
  sm.handle_mg_event(nc, event, event_data);
}


// FreeRTOS task to start Mongoose.
void mg_task(void* data) {
  ESP_LOGD(l_tag, "Task starting");
  struct mg_mgr mgr;

  ESP_LOGD(l_tag, "Starting setup");
  mg_mgr_init(&mgr, NULL);

  ESP_LOGD(l_tag, "Succesfully inited");
  struct mg_connection *c = mg_bind(&mgr, ":80", mongoose_event_handler);

  ESP_LOGD(l_tag, "Successfully bound");
  if (c == NULL) {
    ESP_LOGE(l_tag, "No connection from the mg_bind()");
    vTaskDelete(NULL);
    return;
  }
  mg_set_protocol_http_websocket(c);

  while (1) {
    mg_mgr_poll(&mgr, 1000);
  }
}

// ESP32 WiFi handler.
static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    esp_wifi_connect();
    ESP_LOGI(l_tag, "retry to connect to the AP");
    ESP_LOGI(l_tag,"connect to the AP fail");
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
    ESP_LOGI(l_tag, "got ip:%s", ip4addr_ntoa(&event->ip_info.ip));
    xTaskCreatePinnedToCore(&mg_task, "mg_task", 20000, NULL, 5, NULL,0);
    config_mdns();
  }
}


void init_wifi() {
  tcpip_adapter_init();

  ESP_ERROR_CHECK(esp_event_loop_create_default());

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

  ESP_LOGI(l_tag, "wifi_init_sta finished.");
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
