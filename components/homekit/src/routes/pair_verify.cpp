#include "routes/pair_verify.hpp"

#include <vector>
#include <esp_log.h>

#include "srp.hpp"
#include "cryptor.hpp"
#include "hkdf.hpp"
#include "ed25519.hpp"
#include "curve25519.hpp"
#include "host_info.hpp"
#include "key_storage.hpp"
#include "pairing_manager.hpp"
#include "utils.hpp"

using namespace std;

namespace routes {

void PairVerify::handle_request(Request& request, std::list<Accessory>& accessories) {
  // Load the pair progress if any
  auto& verify_state = _verify_states[request.get_session().get_identifier()];

  // Get the verify state from the request
  auto req_tlvs = TLV::decode(request.get_body());
  auto state_tlv = req_tlvs.find(HAP_TLV_TYPE_STATE);
  if(state_tlv) {
    uint8_t hap_state = (uint8_t)state_tlv->get_value()[0];
    ESP_LOGD("pair-verify", "Got state from client: M%u", hap_state);
    verify_state.stage = (VerifyStage)(hap_state - 1);
  }

  ESP_LOGD("pair-verify", "Got %u TLVs from client", req_tlvs.get().size());

  switch(verify_state.stage) {
    case VerifyStage::M0 : handle_m1(request, req_tlvs, verify_state); break;
    case VerifyStage::M2 : handle_m3(request, req_tlvs, verify_state); break;
    default : break;
  }
}

void PairVerify::handle_m1(Request& request, TLVs& tlvs, VerifyState& state) {
  ESP_LOGI("pair-verify", "Handling M1");
  auto public_key = tlvs.find(HAP_TLV_TYPE_PUB_KEY);
  if(!public_key) {
    ESP_LOGE("pair-verify", "Device failed to send pub key");
    request.get_session().close();
  }
  state.device_public_key = public_key->get_value();

  // Generate session key
  tie(state.accessory_public_key, state.accessory_private_key) = Curve25519::gen_keypair();
  state.shared_secret = Curve25519::compute_shared_secret(
    state.accessory_private_key,
    state.device_public_key
  );

  // Build accessory_info
  string accessory_info = state.accessory_public_key + get_mac_address() + state.device_public_key;

  // Sign accessory_info
  string signature = sign_ed25519(accessory_info, get_private_key());

  // Build sub tlvs
  string sub_tlv;
  sub_tlv += TLV(HAP_TLV_TYPE_IDENTIFIER, get_mac_address()).serialize();
  sub_tlv += TLV(HAP_TLV_TYPE_SIGNATURE, signature).serialize();

  // Get symetric session key
  state.session_key = hkdf_sha512(
    state.shared_secret,
    "Pair-Verify-Encrypt-Salt",
    "Pair-Verify-Encrypt-Info",
    32
  );

  // Encrypt sub tlv
  Cryptor cryptor(state.session_key, "PV-Msg02");
  string encrypted_sub_tlv = cryptor.encrypt(sub_tlv);

  // Build wrapper tlv and send
  string wrapper_resp;
  wrapper_resp += TLV(HAP_TLV_TYPE_STATE, {0x02}).serialize();
  wrapper_resp += TLV(HAP_TLV_TYPE_PUB_KEY, state.accessory_public_key).serialize();
  wrapper_resp += TLV(HAP_TLV_TYPE_ENCRYPTED_DATA, encrypted_sub_tlv).serialize();
  request.get_session().send(200, wrapper_resp, "application/pairing+tlv8");

  state.stage = VerifyStage::M3;
}

void PairVerify::handle_m3(Request& request, TLVs& tlvs, VerifyState& state) {
  ESP_LOGI("pair-verify", "Handling M3");
  auto encrypted_data = tlvs.find(HAP_TLV_TYPE_ENCRYPTED_DATA);
  if(!encrypted_data) {
    ESP_LOGE("pair-verify", "Device failed to send encrypted data");
    request.get_session().close();
    return;
  }

  // Decrypt and verify message from device
  Cryptor cryptor(state.session_key, "PV-Msg03");
  auto plain_text = cryptor.decrypt(encrypted_data->get_value());
  if(!plain_text) {
    ESP_LOGE("pair-verify", "Device failed to send encrypted data");
    string resp;
    resp += TLV(HAP_TLV_TYPE_STATE, {0x04}).serialize();
    resp += TLV(HAP_TLV_TYPE_ERROR, {HAP_TLV_ERROR_AUTH}).serialize();
    request.get_session().send(200, resp, "application/pairing+tlv8");
    return;
  }

  // Parse sub tlv
  auto sub_tlvs = TLV::decode(*plain_text);
  auto pairing_id = sub_tlvs.find(HAP_TLV_TYPE_IDENTIFIER);
  auto signature = sub_tlvs.find(HAP_TLV_TYPE_SIGNATURE);
  if(!pairing_id || !signature) {
    ESP_LOGE("pair-verify", "Encrypted TLV doesn't have the required types");
    request.get_session().close();
    return;
  }

  // Find the pairing if any
  PairingManager pm;
  auto pairing = pm.find_pairing(pairing_id->get_value());
  if(!pairing) {
    ESP_LOGE("pair-verify", "Device pairing not found");
    string resp;
    resp += TLV(HAP_TLV_TYPE_STATE, {0x04}).serialize();
    resp += TLV(HAP_TLV_TYPE_ERROR, {HAP_TLV_ERROR_AUTH}).serialize();
    request.get_session().send(200, resp, "application/pairing+tlv8");
    return;
  }

  // Verify signature
  string iosdevice_info = state.device_public_key + pairing_id->get_value() + state.accessory_public_key;
  if(!verify_ed25519(signature->get_value(), iosdevice_info, pairing->get_public_key())) {
    ESP_LOGE("pair-verify", "Device signature not valid");
    string resp;
    resp += TLV(HAP_TLV_TYPE_STATE, {0x04}).serialize();
    resp += TLV(HAP_TLV_TYPE_ERROR, {HAP_TLV_ERROR_AUTH}).serialize();
    request.get_session().send(200, resp, "application/pairing+tlv8");
    return;
  }

  ESP_LOGI("pair-verify", "Succesfully verified device: %s", pairing_id->get_value().c_str());

  // Send positive response
  string resp;
  resp += TLV(HAP_TLV_TYPE_STATE, {0x04}).serialize();
  request.get_session().send(200, resp, "application/pairing+tlv8");

  // Setup the session
  request.get_session().setup_security(state.shared_secret, pairing->is_admin());
}

} // namespace routes
