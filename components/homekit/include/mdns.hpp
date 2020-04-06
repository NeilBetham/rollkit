#include <mdns.h>
#include <esp_log.h>
#include <string.h>

#include "host_info.hpp"

static char mdns_l_tag[] = "mdns";

// Damn you ISO C++!!!
static const char* config_number_key = "c#";
static const char* config_number_value = "2";

static const char* feature_flags_key = "ff";
static const char* feature_flags_value = "0";

static const char* device_id_key = "id";
static const char* device_id_value = "00:00:00:00:00:00";

static const char* model_name_key = "md";
static const char* model_name_value = "";

static const char* proto_ver_key = "pv";
static const char* proto_ver_value = "1.0";

static const char* state_number_key = "s#";
static const char* state_number_value = "1";

static const char* status_flags_key = "sf";
static const char* status_flags_value = "1";

static const char* cat_id_key = "ci";
static const char* cat_id_value = "1";


static mdns_txt_item_t hap_txt_data[8] = {
  {(char*)config_number_key, (char*)config_number_value},
  {(char*)feature_flags_key, (char*)feature_flags_value},
  {(char*)device_id_key, (char*)device_id_value},
  {(char*)model_name_key, (char*)model_name_value},
  {(char*)proto_ver_key, (char*)proto_ver_value},
  {(char*)state_number_key, (char*)state_number_value},
  {(char*)status_flags_key, (char*)status_flags_value},
  {(char*)cat_id_key, (char*)cat_id_value},
};


void config_mdns(std::string accessory_name, std::string accessory_model){
  esp_err_t err = mdns_init();
  if (err) {
      printf("MDNS Init failed: %d\n", err);
      return;
  }

  // Set proper mac address
  char* mac_addr = (char*)malloc(sizeof(char) * 18);
  auto dc = strcpy(mac_addr, get_mac_address().c_str());
  hap_txt_data[2].value = mac_addr;

  // Set model name
  char* model_addr = (char*)malloc(sizeof(char) * accessory_model.size());
  dc = strcpy(model_addr, accessory_model.c_str());
  hap_txt_data[3].value = model_addr;

  ESP_ERROR_CHECK(mdns_hostname_set(accessory_name.c_str()));
  ESP_ERROR_CHECK(mdns_service_add(accessory_name.c_str(), "_hap", "_tcp", 80, hap_txt_data, 8));

  ESP_LOGD(mdns_l_tag, "MDNS Config Complete");
}
