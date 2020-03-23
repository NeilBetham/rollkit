#include "pairing_manager.hpp"

#include <cstring>
#include <esp_log.h>
#include <sodium.h>

using namespace std;

namespace {

constexpr size_t pairing_id_bytes = 36;
constexpr size_t public_key_bytes = crypto_sign_PUBLICKEYBYTES;
const string nvs_key_pairing_prefix = "p.";
const string nvs_key_pair_count = "p.count";

struct PairData {
  char id[pairing_id_bytes];
  char pk[public_key_bytes];
};

} // namespace


bool PairingManager::add_pairing(const Pairing& pairing) {
  // Check if a pairing already exists for this device
  auto previous_pairing = find_pairing_key(pairing.get_pairing_id());
  if(previous_pairing) {
    ESP_LOGI("pair-mgr", "Removing old pairing device: %s", pairing.get_pairing_id().c_str());
    _nvsmgr.erase_key(*previous_pairing);
  }

  // Get the key for the new pairing
  string key = get_pairing_key();
  PairData pd;
  memset(&pd, 0, sizeof(pd));

  // Copy pairing ID over
  string p_id = pairing.get_pairing_id();
  memcpy(&pd.id, p_id.data(), p_id.size());

  // Copy public key over
  string p_key = pairing.get_public_key();
  memcpy(&pd.pk, p_key.data(), p_key.size());

  // Serialize struct
  string blob((char*)&pd, sizeof(pd));

  // Store the pairing data
  return _nvsmgr.write_blob(key, blob);
}

bool PairingManager::remove_pairing(const std::string& pairing_id) {
  auto pairing_nvs_key = find_pairing_key(pairing_id);
  if(!pairing_nvs_key) { return false; }
  return _nvsmgr.erase_key(pairing_nvs_key.value());
}

optional<Pairing> PairingManager::find_pairing(const std::string& pairing_id) {
  auto pairing_nvs_key = find_pairing_key(pairing_id);
  if(!pairing_nvs_key) { return {}; }
  PairData pd;

  // Read the pairing data
  auto pair_data = _nvsmgr.read_blob(*pairing_nvs_key);
  if(!pair_data) { return {}; }
  memcpy(&pd, pair_data->data(), pair_data->size());

  return Pairing({pd.id}, {pd.pk});
}

string PairingManager::get_pairing_key() {
  uint32_t pair_count = 0;
  auto nvs_pair_count = _nvsmgr.read_uint32(nvs_key_pair_count);
  if(nvs_pair_count){ pair_count = nvs_pair_count.value(); }
  _nvsmgr.write_uint32(nvs_key_pair_count, ++pair_count);

  return nvs_key_pairing_prefix + to_string(pair_count);
}

optional<string> PairingManager::find_pairing_key(const string& pairing_id) {
  nvs_iterator_t nvs_iter = nvs_entry_find("nvs", storage_namespace.c_str(), NVS_TYPE_BLOB);
  nvs_entry_info_t entry_info;

  while(nvs_iter != NULL) {
    PairData pd;
    nvs_entry_info(nvs_iter, &entry_info);

    // Read the pairing data and check if the id matches
    auto pair_data = _nvsmgr.read_blob(entry_info.key);
    if(pair_data) {
      memcpy(&pd, pair_data->data(), pair_data->size());
      ESP_LOGI("pair-mgr", "Checking pairing: %s", string(pd.id, pairing_id_bytes).c_str());

      if(string(pd.id, pairing_id_bytes) == pairing_id) { return {entry_info.key}; }
    }

    nvs_iter = nvs_entry_next(nvs_iter);
  }

  return {};
}
