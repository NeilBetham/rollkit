#include "routes/pair_setup.hpp"

#include <vector>

#include "srp.hpp"
#include "esp_log.h"

using namespace std;

namespace routes {


void PairSetup::handle_request(Request& request) {
  auto req_tlvs = TLV::decode(request.get_body());
  auto state_tlv = req_tlvs.find(HAP_TLV_TYPE_STATE);
  if(state_tlv) {
    uint8_t hap_state = (uint8_t)state_tlv->get_value()[0];
    ESP_LOGD("pair-setup", "Got state from client: M%u", hap_state);
    _setup_stage = (PairState)(hap_state - 1);
  }

  ESP_LOGD("pair-setup", "Got %u TLVs from client", req_tlvs.get().size());

  switch(_setup_stage) {
    case PairState::M0 : handle_m1(request, req_tlvs); break;
    case PairState::M2 : handle_m3(request, req_tlvs); break;
    case PairState::M4 : handle_m5(request, req_tlvs); break;
    default : break;
  }
}


void PairSetup::handle_m1(Request& request, TLVs& tlvs) {
  ESP_LOGI("pair-setup", "Handling M1");
  auto hap_method = tlvs.find(HAP_TLV_TYPE_METHOD);

  // Check we are receiving the correct setup params
  if(hap_method && (hap_method->get_value()[0] != HAP_TLV_METHOD_PAIR_SETUP_WITH_AUTH)) {
    ESP_LOGE("pair-setup", "Client requesting unsupported pair setup with alternate auth");
    reset();
    request.get_session().close();
    return;
  }

  // Generate challenge
  auto challenge = _srp_verifier.get_challenge();
  string resp;
  resp += TLV(HAP_TLV_TYPE_STATE, {0x02}).serialize();
  resp += TLV(HAP_TLV_TYPE_PUB_KEY, challenge.get_B().export_raw()).serialize();
  resp += TLV(HAP_TLV_TYPE_SALT, challenge.get_salt().export_raw()).serialize();

  // Send TVLs to client
  ESP_LOGD("pair-setup", "Sending TLV response to client");
  request.get_session().send(200, resp, "application/pairing+tlv8");

  // Set PairState
  _setup_stage = PairState::M2;
}

void PairSetup::handle_m3(Request& request, TLVs& tlvs) {
  ESP_LOGI("pair-setup", "Handling M3");

  // Pull the PubKey and proof from the TLVs
  auto ios_pub_key = tlvs.find(HAP_TLV_TYPE_PUB_KEY);
  auto ios_proof   = tlvs.find(HAP_TLV_TYPE_PROOF);

  if(!ios_pub_key || !ios_proof) {
    ESP_LOGE("pair-setup", "iOS Device didn't send proper M3 TLVs");
    reset();
    request.get_session().close();
    return;
  }
  BigNum pub_key = BigNum::from_raw(ios_pub_key->get_value());
  BigNum proof   = BigNum::from_raw(ios_proof->get_value());

  ESP_LOGD("pair-setup", "iOS Pub Key: 0x%s", pub_key.export_b16().c_str());
  ESP_LOGD("pair-setup", "iOS Proof: 0x%s", proof.export_b16().c_str());


  // Update the verifier with the pub key and proof and see if they checkout
  bool verified = _srp_verifier.verify(pub_key, proof);
  if(!verified) {
    ESP_LOGE("pair-setup", "iOS Device failed authentication");
    string resp;
    resp += TLV(HAP_TLV_TYPE_STATE, {0x04}).serialize();
    resp += TLV(HAP_TLV_TYPE_ERROR, {HAP_TLV_ERROR_AUTH}).serialize();
    request.get_session().send(200, resp, "application/pairing+tlv8");
    reset();
    return;
  }

  // Send Proof to client
  string resp;
  resp += TLV(HAP_TLV_TYPE_STATE, {0x04}).serialize();
  resp += TLV(HAP_TLV_TYPE_PROOF, _srp_verifier.get_client_proof().export_raw()).serialize();
  request.get_session().send(200, resp, "application/pairing+tlv8");

  // Set PairState
  _setup_stage = PairState::M4;
}

void PairSetup::handle_m5(Request& request, TLVs& tlvs) {
  ESP_LOGI("pair-setup", "Handling M5");
  ESP_LOGD("pair-setup", "TLVs:");
  for(auto& tlv : tlvs.get()) {
    ESP_LOGD("pair-setup", "Type %u, Value %s", tlv.get_type(), tlv.get_value_hex().c_str());
  }
}


} // namespace routes
