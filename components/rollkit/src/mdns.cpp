#include "rollkit/mdns.hpp"

#include <esp_log.h>
#include <string.h>

#include "rollkit/host_info.hpp"
#include "rollkit/pairing_manager.hpp"

namespace rollkit {


static const char* config_number_key = "c#";
static const char* device_id_key = "id";
static const char* model_name_key = "md";
static const char* proto_ver_key = "pv";
static const char* state_number_key = "s#";
static const char* status_flags_key = "sf";
static const char* cat_id_key = "ci";

static const char* config_number_value = "2";
static const char* proto_ver_value = "1.1";
static const char* state_number_value = "1";
static const char* cat_id_value = "5";


MDNSManager::MDNSManager(std::string acc_name, std::string acc_model, std::string mac) :
    _acc_name(acc_name), _acc_model(acc_model), _device_id(mac) {

  // Init mdns server
  esp_err_t err = mdns_init();
  if (err) {
    printf("MDNS Init failed: %d\n", err);
    return;
  }

  // Set the hostname and serivce info
  ESP_LOGD("mdns-mgr", "Setting hostname: %s", _acc_name.c_str());
  ESP_ERROR_CHECK(mdns_hostname_set(_acc_name.c_str()));
  ESP_ERROR_CHECK(mdns_service_add(_acc_name.c_str(), "_hap", "_tcp", 80, NULL, 0));

  publish_record();
}

void MDNSManager::publish_record() {
  update_state();

  // Populate the hap txt records
  _hap_txt_data[0] = {config_number_key, config_number_value};
  _hap_txt_data[1] = {device_id_key, _device_id.c_str()};
  _hap_txt_data[2] = {model_name_key, _acc_model.c_str()};
  _hap_txt_data[3] = {proto_ver_key, proto_ver_value};
  _hap_txt_data[4] = {state_number_key, state_number_value};
  _hap_txt_data[5] = {status_flags_key, _status_flag.c_str()};
  _hap_txt_data[6] = {cat_id_key, cat_id_value};

  // Set the records
  mdns_service_txt_set("_hap", "_tcp", _hap_txt_data, 7);


  ESP_LOGD("mdns-mgr", "MDNS Config Complete");
}

void MDNSManager::update_state() {
  PairingManager pm;
  _status_flag = pm.get_pairing_count() > 0 ? "0" : "1";
}


} // namespace rollkit
