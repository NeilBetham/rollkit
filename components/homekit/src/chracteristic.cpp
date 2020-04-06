#include "characteristic.hpp"

#include "utils.hpp"

nlohmann::json Characteristic::serialize() const {
  nlohmann::json ret;

  ret["type"] = _hap_type;
  ret["iid"] = _instance_id;
  ret["value"] = handle_read();
  ret["perms"] = _perms;
  ret["format"] = _format;

  return ret;
}
