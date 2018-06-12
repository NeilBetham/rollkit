#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <esp_event.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <nvs_flash.h>
#include <mdns.h>

#include "config.hpp"

#include "mongoose.h"
#include "sdkconfig.h"

static char l_tag []="http";

// Convert a Mongoose string type to a string.
char *mg_str_to_str(struct mg_str mgStr) {
  char *retStr = (char *) malloc(mgStr.len + 1);
  memcpy(retStr, mgStr.p, mgStr.len);
  retStr[mgStr.len] = 0;
  return retStr;
}

// Simple Example HTTP event handler
void mongoose_event_handler(struct mg_connection *nc, int ev, void *evData) {
  switch (ev) {
    case MG_EV_HTTP_REQUEST: {
      ESP_LOGD(l_tag, "Got http request");
      struct http_message *message = (struct http_message *) evData;

      char *uri = mg_str_to_str(message->uri);

      if (strcmp(uri, "/time") == 0) {
        char payload[256];
        struct timeval tv;
        gettimeofday(&tv, NULL);
        sprintf(payload, "Time since start: %d.%d secs", (int)tv.tv_sec, (int)tv.tv_usec);
        int length = strlen(payload);
        mg_send_head(nc, 200, length, "Content-Type: text/plain");
        mg_printf(nc, "%s", payload);
      }  else {
        mg_send_head(nc, 404, 0, "Content-Type: text/plain");
      }
      nc->flags |= MG_F_SEND_AND_CLOSE;
      free(uri);
      break;
    }
  }
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


void config_mdns(){
  esp_err_t err = mdns_init();
  if (err) {
      printf("MDNS Init failed: %d\n", err);
      return;
  }

  mdns_hostname_set(ACC_NAME);

  mdns_txt_item_t hap_txt_data[8] = {
      {"c#", "2"},
      {"ff", "1"},
      {"id", "32:09:CD:BD:FF:AC"},
      {"md", ACC_MODEL},
      {"pv", "1.0"},
      {"s#", "1"},
      {"sf", "1"},
      {"ci", "1"}
  };
  mdns_service_add(ACC_NAME, "_hap", "_tcp", 80, hap_txt_data, 8);
}

// ESP32 WiFi handler.
esp_err_t wifi_event_handler(void *ctx, system_event_t *event) {
  if (event->event_id == SYSTEM_EVENT_STA_GOT_IP) {
    ESP_LOGD(l_tag, "Got an IP: " IPSTR, IP2STR(&event->event_info.got_ip.ip_info.ip));
    xTaskCreatePinnedToCore(&mg_task, "mg_task", 20000, NULL, 5, NULL,0);
    config_mdns();
  }

  if (event->event_id == SYSTEM_EVENT_STA_START) {
    ESP_LOGD(l_tag, "Received a start request");
    ESP_ERROR_CHECK(esp_wifi_connect());
  }

  return ESP_OK;
}


int app_main(void) {
  nvs_flash_init();
  tcpip_adapter_init();
  ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  wifi_config_t sta_config = {
      .sta = {
          .ssid = WIFI_SSID,
          .password = WIFI_PASS,
          .bssid_set = 0
      }
  };
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_connect());

  return 0;
} // app_main
