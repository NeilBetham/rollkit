#include "rollkit/routes/pair_setup.hpp"

#include <vector>
#include <esp_log.h>

#include "srp.hpp"
#include "rollkit/cryptor.hpp"
#include "rollkit/hkdf.hpp"
#include "rollkit/ed25519.hpp"
#include "rollkit/key_storage.hpp"
#include "rollkit/pairing_manager.hpp"

using namespace std;

namespace rollkit {
namespace routes {


void PairSetup::handle_request(Request& request, interfaces::IApp& app) {
  auto req_tlvs = TLV::decode(request.get_body());
  auto state_tlv = req_tlvs.find(HAP_TLV_TYPE_STATE);
  if(state_tlv) {
    uint8_t hap_state = (uint8_t)state_tlv->get_value()[0];
    ESP_LOGD("pair-setup", "Got state from client: M%u", hap_state);
    _setup_stage = (PairState)(hap_state - 1);
  }

  ESP_LOGD("pair-setup", "Got %u TLVs from client", req_tlvs.get().size());

  switch(_setup_stage) {
    case PairState::M0 : handle_m1(request, req_tlvs, app); break;
    case PairState::M2 : handle_m3(request, req_tlvs); break;
    case PairState::M4 : handle_m5(request, req_tlvs, app); break;
    default : break;
  }
}


void PairSetup::handle_m1(Request& request, TLVs& tlvs, interfaces::IApp& app) {
  ESP_LOGI("pair-setup", "Handling M1");
  auto hap_method = tlvs.find(HAP_TLV_TYPE_METHOD);
  ESP_LOGD("pair-setup", "Pairing setup code: `%s`", app.get_setup_code().c_str());

  // Check we are receiving the correct setup params
  if(hap_method && (hap_method->get_value()[0] != HAP_TLV_METHOD_PAIR_SETUP) && (hap_method->get_value()[0] != HAP_TLV_METHOD_PAIR_SETUP_WITH_AUTH)) {
    ESP_LOGE("pair-setup", "Client requesting unsupported pair setup: %d", hap_method->get_value()[0]);
    reset();
    request.get_session().close();
    return;
  }

  // Check if we are pairing with another controller
  if(_pair_in_progress) {
    string resp;
    resp += TLV(HAP_TLV_TYPE_STATE, {0x02}).serialize();
    resp += TLV(HAP_TLV_TYPE_ERROR, {HAP_TLV_ERROR_BUSY}).serialize();
    request.get_session().send(200, resp, "application/pairing+tlv8");
    return;
  }
  _pair_in_progress = true;
  _pairing_session = request.get_session().get_identifier();
  request.get_session().register_event_listener(this);

  // Check if a controller is paired already
  PairingManager pm;
  if(pm.get_pairing_count() > 0) {
    string resp;
    resp += TLV(HAP_TLV_TYPE_STATE, {0x02}).serialize();
    resp += TLV(HAP_TLV_TYPE_ERROR, {HAP_TLV_ERROR_UNAVAILABLE}).serialize();
    request.get_session().send(200, resp, "application/pairing+tlv8");
    reset();
    return;
  }

  // Setup SRP info
  _srp_user = SRP::User::fromPassword(_srp_math, "Pair-Setup", app.get_setup_code());
  _srp_verifier = SRP::Verifier(_srp_math, _srp_user);

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

void PairSetup::handle_m5(Request& request, TLVs& tlvs, interfaces::IApp& app) {
  ESP_LOGI("pair-setup", "Handling M5");

  // Get the encrypted TLV
  auto encrypted_tlv = tlvs.find(HAP_TLV_TYPE_ENCRYPTED_DATA);
  if(!encrypted_tlv) {
    ESP_LOGE("pair-setup", "No encrypted TLV found");
    reset();
    request.get_session().close();
    return;
  }

  // Decrypt TLV
  auto srp_shared_secret = _srp_verifier.get_shared_secret().export_raw();
  string session_key = hkdf_sha512(
    srp_shared_secret,
    "Pair-Setup-Encrypt-Salt",
    "Pair-Setup-Encrypt-Info",
    crypto_aead_chacha20poly1305_IETF_KEYBYTES
  );
  Cryptor cryptor(session_key, "PS-Msg05");
  auto decrypted_tlv = cryptor.decrypt(encrypted_tlv->get_value());
  if(!decrypted_tlv) {
    ESP_LOGE("pair-setup", "Failed to authenticate encrypted tlv");
    string resp;
    resp += TLV(HAP_TLV_TYPE_STATE, {0x06}).serialize();
    resp += TLV(HAP_TLV_TYPE_ERROR, {HAP_TLV_ERROR_AUTH}).serialize();
    request.get_session().send(200, resp, "application/pairing+tlv8");
    reset();
    return;
  }

  // Parse the sub TLV
  auto sub_tlvs = TLV::decode(*decrypted_tlv);
  auto id_tlv = sub_tlvs.find(HAP_TLV_TYPE_IDENTIFIER);
  auto public_key_tlv = sub_tlvs.find(HAP_TLV_TYPE_PUB_KEY);
  auto signature_tlv = sub_tlvs.find(HAP_TLV_TYPE_SIGNATURE);
  if(!id_tlv | !public_key_tlv | !signature_tlv) {
    ESP_LOGE("pair-setup", "Failed to get required sub TLVs");
    reset();
    request.get_session().close();
    return;
  }

  // Generate iosdevice_x
  auto iosdevice_x = hkdf_sha512(
    srp_shared_secret,
    "Pair-Setup-Controller-Sign-Salt",
    "Pair-Setup-Controller-Sign-Info",
    32
  );

  // Build iOSDeviceInfo
  string iosdevice_info = iosdevice_x + id_tlv->get_value() + public_key_tlv->get_value();

  // Verify Signature
  if(!verify_ed25519(signature_tlv->get_value(), iosdevice_info, public_key_tlv->get_value())) {
    ESP_LOGE("pair-setup", "Failed to verify signature");
    string resp;
    resp += TLV(HAP_TLV_TYPE_STATE, {0x06}).serialize();
    resp += TLV(HAP_TLV_TYPE_ERROR, {HAP_TLV_ERROR_AUTH}).serialize();
    request.get_session().send(200, resp, "application/pairing+tlv8");
    reset();
    return;
  }
  ESP_LOGI("pair-setup", "iOS Signature verified");

  // Save the pairing
  Pairing pairing(id_tlv->get_value(), public_key_tlv->get_value());
  PairingManager pm;
  if(!pm.add_pairing(pairing)) {
    ESP_LOGE("pair-setup", "Failed save pairing");
    string resp;
    resp += TLV(HAP_TLV_TYPE_STATE, {0x06}).serialize();
    resp += TLV(HAP_TLV_TYPE_ERROR, {HAP_TLV_ERROR_MAX_PEERS}).serialize();
    request.get_session().send(200, resp, "application/pairing+tlv8");
    reset();
    return;
  }

  // Generate accessory_x
  auto accessory_x = hkdf_sha512(
    srp_shared_secret,
    "Pair-Setup-Accessory-Sign-Salt",
    "Pair-Setup-Accessory-Sign-Info",
    32
  );

  // Build AccessoryInfo
  string accessory_info = accessory_x + app.get_device_id() + get_public_key();

  // Sign the accessory_info
  auto signature = sign_ed25519(accessory_info, get_private_key());

  // Build TLV to send back to client
  string resp;
  resp += TLV(HAP_TLV_TYPE_IDENTIFIER, app.get_device_id()).serialize();
  resp += TLV(HAP_TLV_TYPE_PUB_KEY, get_public_key()).serialize();
  resp += TLV(HAP_TLV_TYPE_SIGNATURE, signature).serialize();

  // Encrypt TLV and send
  cryptor.set_nonce("PS-Msg06");
  auto encrypted_data = cryptor.encrypt(resp);
  string wrapper_resp;
  wrapper_resp += TLV(HAP_TLV_TYPE_STATE, {0x06}).serialize();
  wrapper_resp += TLV(HAP_TLV_TYPE_ENCRYPTED_DATA, encrypted_data).serialize();
  request.get_session().send(200, wrapper_resp, "application/pairing+tlv8");

  // Set PairState
  _setup_stage = PairState::M6;
  _pair_in_progress = false;
  _pairing_session = NULL;
}

void PairSetup::session_closed(void* session_id) {
  if(_pair_in_progress == false) { return; }
  ESP_LOGI("pair-setup", "Session %p closed; canceling in progress pairing", session_id);
  if(_pairing_session == session_id) {
    _pair_in_progress = false;
    _pairing_session = NULL;
  }
}


} // namespace routes
} // namespace rollkit
