#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <esp_event.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <nvs_flash.h>
#include <sodium.h>

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
esp_err_t wifi_event_handler(void *ctx, system_event_t *event) {
  if (event->event_id == SYSTEM_EVENT_STA_GOT_IP) {
    ESP_LOGD(l_tag, "Got an IP: " IPSTR, IP2STR(&event->event_info.got_ip.ip_info.ip));
    xTaskCreatePinnedToCore(&mg_task, "mg_task", 20000, NULL, 5, NULL,0);
    config_mdns();
  }

  if (event->event_id == SYSTEM_EVENT_STA_START) {
    ESP_LOGD(l_tag, "WiFi Started");
    ESP_ERROR_CHECK(esp_wifi_connect());
  }

  if (event->event_id == SYSTEM_EVENT_STA_CONNECTED) {
    ESP_LOGD(l_tag, "Station connected");
  }

  if (event->event_id == SYSTEM_EVENT_STA_DISCONNECTED) {
    ESP_LOGD(l_tag, "Station disconnected");
    printf("reason: %d\n",event->event_info.disconnected.reason);
    ESP_ERROR_CHECK(esp_wifi_connect());
  }

  return ESP_OK;
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
  tcpip_adapter_init();
  ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  wifi_config_t sta_config;
  strcpy((char*)sta_config.sta.ssid, WIFI_SSID);
  strcpy((char*)sta_config.sta.password, WIFI_PASS);
  sta_config.sta.bssid_set = 0;

  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
  ESP_ERROR_CHECK(esp_wifi_start());
}
