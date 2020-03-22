#include "key_storage.hpp"

#include <esp_log.h>
#include <sodium.h>
#include <stdexcept>

#include "nvs_manager.hpp"

using namespace std;

const string pub_key_key = "acc.pub_key";
const string priv_key_key = "acc.priv_key";

namespace {

void init_keys() {
  string pub_key(crypto_sign_PUBLICKEYBYTES, 0);
  string priv_key(crypto_sign_SECRETKEYBYTES, 0);
  crypto_sign_keypair((uint8_t*)&pub_key[0], (uint8_t*)&priv_key[0]);

  NVSManager nvsmgr;
  if(!nvsmgr.write_blob(pub_key_key, pub_key)) {
    ESP_LOGE("key-strg", "Failed to write pub key to nvs");
  }
  if(!nvsmgr.write_blob(priv_key_key, priv_key)) {
    ESP_LOGE("key-strg", "Failed to write priv key to nvs");
  }
}

string read_or_init_key(const string& key) {
  NVSManager nvsmgr;
  auto key_data = nvsmgr.read_blob(key);
  if(!key_data) {
    ESP_LOGI("key-strg", "No keys in NVS, generating new one");
    init_keys();

    key_data = nvsmgr.read_blob(key);
    if(!key_data) {
      throw std::runtime_error("Failed to generate and store keys");
    }
  }

  return key_data.value();
}

} // namespace

std::string get_public_key() {
  return read_or_init_key(pub_key_key);
}

std::string get_private_key() {
  return read_or_init_key(priv_key_key);
}

void clear_keys() {
  NVSManager nvsmgr;
  nvsmgr.erase_key(pub_key_key);
  nvsmgr.erase_key(priv_key_key);
}
