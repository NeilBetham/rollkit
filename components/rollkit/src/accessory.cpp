#include "rollkit/accessory.hpp"

namespace rollkit {


nlohmann::json Accessory::serialize() const {
  nlohmann::json ret;

  std::list<nlohmann::json> services;
  for(const auto& service : _services) {
    services.push_back(service.serialize());
  }

  ret["aid"] = _instance_id;
  ret["services"] = services;

  return ret;
}


} // namespace rollkit
