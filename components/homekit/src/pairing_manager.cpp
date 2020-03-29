#include "pairing_manager.hpp"

#include <cstring>
#include <esp_log.h>
#include <sodium.h>

using namespace std;

namespace {

const string nvs_key_pair_index = "p.count";
const string nvs_key_pairing_prefix = "p";

const size_t max_esp_nvs_key_length = 15;
const string max_int_string((max_esp_nvs_key_length - 1) - nvs_key_pairing_prefix.size(), 9);
const char*  max_int_c_string = max_int_string.c_str();
const uint64_t max_pairing_index = atoll(max_int_c_string);

const size_t pairing_id_bytes = 36;
const size_t public_key_bytes = crypto_sign_PUBLICKEYBYTES;

struct PairData {
  char id[pairing_id_bytes];
  char pk[public_key_bytes];
};

Pairing to_pairing(const PairData& pd) {
  string id(pd.id, pairing_id_bytes);
  string pk(pd.pk, public_key_bytes);
  return {id, pk};
}


} // namespace


bool PairingManager::add_pairing(const Pairing& pairing) {
  // Check if a pairing already exists for this device
  auto previous_pairing = find_pairing_key(pairing.get_pairing_id());
  if(previous_pairing) {
    ESP_LOGI("pair-mgr", "Removing old pairing device: %s", pairing.get_pairing_id().c_str());
    _nvsmgr.erase_key(*previous_pairing);
  }

  // Get the storage key for the new pairing
  string key = get_next_pairing_key();
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
  if(_nvsmgr.write_blob(key, blob)) {
    return inc_pairing_index();
  }
  return false;
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

std::list<Pairing> PairingManager::get_all_pairings() {
  list<Pairing> pairings;
  nvs_iterator_t nvs_iter = nvs_entry_find("nvs", storage_namespace.c_str(), NVS_TYPE_BLOB);
  nvs_entry_info_t entry_info;

  // TODO: DRY this up with find_pairing_key
  while(nvs_iter != NULL) {
    PairData pd;
    nvs_entry_info(nvs_iter, &entry_info);

    // Check that the key has the pairing prefix
    auto key_prefix = string(entry_info.key, nvs_key_pairing_prefix.size());
    if(key_prefix != nvs_key_pairing_prefix) { continue; }

    // Read the pairing data and check if the id matches
    auto pair_data = _nvsmgr.read_blob(entry_info.key);
    if(pair_data) {
      memcpy(&pd, pair_data->data(), pair_data->size());
      pairings.push_back(to_pairing(pd));
    }

    nvs_iter = nvs_entry_next(nvs_iter);
  }

  return pairings;
}

uint32_t PairingManager::get_pairing_count() {
  return get_all_pairings().size();
}

std::string PairingManager::get_next_pairing_key() {
  string key = nvs_key_pairing_prefix;
  key += to_string(get_pairing_index());
  return key;
}

uint32_t PairingManager::get_pairing_index() {
  uint32_t pair_index = 0;
  auto nvs_pair_index = _nvsmgr.read_uint32(nvs_key_pair_index);
  if(nvs_pair_index){ pair_index = nvs_pair_index.value(); }
  return pair_index;
}

bool PairingManager::inc_pairing_index() {
  uint32_t p_index = get_pairing_index();

  if(p_index < max_pairing_index) {
    ++p_index;
  } else {
    p_index = 0;
  }

  return _nvsmgr.write_uint32(nvs_key_pair_index, p_index);
}

optional<string> PairingManager::find_pairing_key(const string& pairing_id) {
  nvs_iterator_t nvs_iter = nvs_entry_find("nvs", storage_namespace.c_str(), NVS_TYPE_BLOB);
  nvs_entry_info_t entry_info;

  while(nvs_iter != NULL) {
    PairData pd;
    nvs_entry_info(nvs_iter, &entry_info);
    nvs_iter = nvs_entry_next(nvs_iter);

    // Check that the key has the pairing prefix
    auto key_prefix = string(entry_info.key, nvs_key_pairing_prefix.size());
    if(key_prefix != nvs_key_pairing_prefix) { continue; }

    // Read the pairing data and check if the id matches
    auto pair_data = _nvsmgr.read_blob(entry_info.key);
    if(pair_data) {
      memcpy(&pd, pair_data->data(), pair_data->size());
      ESP_LOGI("pair-mgr", "Checking pairing: %s", string(pd.id, pairing_id_bytes).c_str());

      if(string(pd.id, pairing_id_bytes) == pairing_id) { return {entry_info.key}; }
    }

  }

  return {};
}
