#include "nvs_manager.hpp"

#include <esp_log.h>

using namespace std;

NVSManager::NVSManager() : _valid(true) {
  // Open NVS
  esp_err_t err = nvs_open(storage_namespace.c_str(), NVS_READWRITE, &_nvs_handle);
  if(err != ESP_OK) { ESP_LOGE("nvs-mgr", "Failed to open NVS"); _valid = false; }
}

NVSManager::~NVSManager() {
  if(!_valid) { return; }

  // Close the nvs handle
  nvs_close(_nvs_handle);
}

bool NVSManager::write_blob(const std::string& key, const std::string& blob) {
  if(!_valid) { return false; }
  esp_err_t err;

  // Write blob
  err = nvs_set_blob(_nvs_handle, key.c_str(), blob.data(), blob.size());
  if(err != ESP_OK) { ESP_LOGE("nvs-mgr", "Failed to write blob to NVS Key: %s", key.c_str()); return false; }

  // Commit the changes
  err = nvs_commit(_nvs_handle);
  if(err != ESP_OK) { ESP_LOGE("nvs-mgr", "Failed to commit NVS changes Key: %s", key.c_str()); return false; }

  return true;
}

optional<std::string> NVSManager::read_blob(const std::string& key) {
  if(!_valid) { return {}; }
  esp_err_t err;
  std::string blob_data;

  // Get size of blob to read
  size_t blob_size = 0;
  err = nvs_get_blob(_nvs_handle, key.c_str(), NULL, &blob_size);
  if(err == ESP_ERR_NVS_NOT_FOUND) { ESP_LOGI("nvs-mgr", "Key not found: %s", key.c_str()); return {}; }
  blob_data.resize(blob_size);

  // Read the blob contents
  blob_size = blob_data.size();
  err = nvs_get_blob(_nvs_handle, key.c_str(), (void*)&blob_data[0], &blob_size);
  if(err != ESP_OK) { ESP_LOGI("nsv-mgr", "Failed to read blob: %s", key.c_str()); return {}; }

  return blob_data;
}

bool NVSManager::write_uint32(const std::string& key, const uint32_t value) {
  if(!_valid) { return {}; }
  esp_err_t err = nvs_set_u32(_nvs_handle, key.c_str(), value);
  if(err != ESP_OK) { ESP_LOGI("nsv-mgr", "Failed to set u32 Key: %s", key.c_str()); return false; }

  return true;
}

optional<uint32_t> NVSManager::read_uint32(const std::string& key) {
  if(!_valid) { return {}; }
  uint32_t value = 0;
  esp_err_t err = nvs_get_u32(_nvs_handle, key.c_str(), &value);
  if(err != ESP_OK) { ESP_LOGI("nsv-mgr", "Failed to get u32 Key: %s", key.c_str()); return {}; }

  return {value};
}

bool NVSManager::erase_key(const string& key) {
  if(!_valid) { return {}; }
  esp_err_t err;

  // Erase the key
  err = nvs_erase_key(_nvs_handle, key.c_str());
  if(err != ESP_OK) { return false; }
  return true;
}
