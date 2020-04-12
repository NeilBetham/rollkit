#include "rollkit/host_info.hpp"

#include <esp_wifi.h>

using namespace std;

namespace rollkit {


string get_mac_address() {
  string ret(17, 0);
  uint8_t mac[6];
  esp_wifi_get_mac(ESP_IF_WIFI_STA, mac);
  sprintf(&ret[0], "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return ret;
}


} // namespace rollkit
