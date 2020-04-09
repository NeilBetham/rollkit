#include "characteristic.hpp"

#include <algorithm>

#include "utils.hpp"

using namespace std;

nlohmann::json Characteristic::serialize() const {
  nlohmann::json ret;
  bool is_readable = find(_perms.begin(), _perms.end(), "pr") != _perms.end();

  ret["type"] = _hap_type;
  ret["iid"] = _instance_id;
  ret["perms"] = _perms;
  ret["format"] = _format;
  if(is_readable) {
    ret["value"] = handle_read();
  }

  return ret;
}
