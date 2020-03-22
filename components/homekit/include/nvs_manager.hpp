#ifndef NVS_MANAGER_HPP
#define NVS_MANAGER_HPP

#include <string>
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "optional.hpp"

const std::string storage_namespace = "hap";

class NVSManager {
public:
  NVSManager();
  ~NVSManager();

  bool write_blob(const std::string& key, const std::string& blob);
  optional<std::string> read_blob(const std::string& key);
  bool erase_key(const std::string& key);

private:
  bool _valid;
  nvs_handle_t _nvs_handle;
};


#endif // NVS_MANAGER_HPP
