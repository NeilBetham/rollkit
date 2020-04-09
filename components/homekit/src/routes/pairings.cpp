#include "routes/pairings.hpp"

#include <esp_log.h>

#include "pairing_manager.hpp"

using namespace std;

namespace routes {


void Pairings::handle_request(Request& request, AccessoryDB& acc_db) {
  auto req_tlvs = TLV::decode(request.get_body());
  auto method = req_tlvs.find(HAP_TLV_TYPE_METHOD);
  if(!method) { request.get_session().close(); return; }

  switch(method->get_value()[0]) {
    case HAP_TLV_METHOD_ADD_PAIRING: handle_add_pairing(request, req_tlvs); break;
    case HAD_TLV_METHOD_REMOVE_PAIRING: handle_remove_pairing(request, req_tlvs); break;
    case HAP_TLV_METHOD_LIST_PAIRINGS: handle_list_pairing(request, req_tlvs); break;
    default: request.get_session().close(); break;
  }
}

void Pairings::handle_add_pairing(Request& request, TLVs& tlvs) {
  ESP_LOGI("pairings", "Add pairing requested");
  auto cont_id = tlvs.find(HAP_TLV_TYPE_IDENTIFIER);
  auto cont_pub_key = tlvs.find(HAP_TLV_TYPE_PUB_KEY);
  if(!cont_id || !cont_pub_key) {
    ESP_LOGE("pair-add", "Controller failed to provide id or pub key");
    request.get_session().close();
    return;
  }

  PairingManager pm;
  Pairing new_p(cont_id->get_value(), cont_pub_key->get_value());
  if(!pm.add_pairing(new_p)) {
    string resp;
    resp += TLV(HAP_TLV_TYPE_STATE, {0x02}).serialize();
    resp += TLV(HAP_TLV_TYPE_ERROR, {HAP_TLV_ERROR_UNKOWN}).serialize();
    ESP_LOGE("pair-add", "Failed to add new pairing");
    request.get_session().send(200, resp, "application/pairing+tlv8");
  }

  ESP_LOGI("pair-add", "Added pairing for controller: %s", cont_id->get_value().c_str());
  string resp;
  resp += TLV(HAP_TLV_TYPE_STATE, {0x02}).serialize();
  request.get_session().send(200, resp, "application/pairing+tlv8");
}

void Pairings::handle_remove_pairing(Request& request, TLVs& tlvs) {
  ESP_LOGI("pairings", "Remove pairing requested");

  auto cont_id = tlvs.find(HAP_TLV_TYPE_IDENTIFIER);
  if(!cont_id) {
    ESP_LOGE("pair-add", "Controller failed to provide id");
    request.get_session().close();
    return;
  }

  PairingManager pm;
  if(!pm.remove_pairing(cont_id->get_value())) {
    string resp;
    resp += TLV(HAP_TLV_TYPE_STATE, {0x02}).serialize();
    resp += TLV(HAP_TLV_TYPE_ERROR, {HAP_TLV_ERROR_UNKOWN}).serialize();
    ESP_LOGE("pair-add", "Failed to remove pairing: %s", cont_id->get_value().c_str());
    request.get_session().send(200, resp, "application/pairing+tlv8");
  }

  ESP_LOGI("pair-add", "Removed pairing for controller: %s", cont_id->get_value().c_str());
  string resp;
  resp += TLV(HAP_TLV_TYPE_STATE, {0x02}).serialize();
  request.get_session().send(200, resp, "application/pairing+tlv8");
}

void Pairings::handle_list_pairing(Request& request, TLVs& tlvs) {
  ESP_LOGI("pairings", "List pairings requested");

  PairingManager pm;
  string resp;

  resp += TLV(HAP_TLV_TYPE_STATE, {0x02}).serialize();
  for(auto& pairing : pm.get_all_pairings()) {
    resp += TLV(HAP_TLV_TYPE_IDENTIFIER, pairing.get_pairing_id()).serialize();
    resp += TLV(HAP_TLV_TYPE_PUB_KEY, pairing.get_public_key()).serialize();
    resp += TLV(HAP_TLV_TYPE_PERMS, {0x00}).serialize();
    resp += TLV(HAP_TLV_TYPE_SEPARATOR, {}).serialize();
  }

  request.get_session().send(200, resp, "application/pairing+tlv8");
}


} // namespace routes
