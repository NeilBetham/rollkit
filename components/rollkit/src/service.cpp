#include "rollkit/service.hpp"

#include <list>

#include "rollkit/utils.hpp"

namespace rollkit {


nlohmann::json Service::serialize() const {
  nlohmann::json ret;

  std::list<nlohmann::json> characteristics;
  for(const auto& characteristic : _characteristics) {
    characteristics.push_back(characteristic.serialize());
  }

  ret["type"] = _hap_type;
  ret["iid"] = _instance_id;
  ret["characteristics"] = characteristics;
  ret["hidden"] = _hidden;
  ret["primary"] = _primary;
  ret["linked"] = _linked_serivces;

  return ret;
}


} // namespace rollkit
