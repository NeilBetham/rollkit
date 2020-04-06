#include "routes/accessories.hpp"

#include "json.hpp"

namespace routes {

void Accessories::handle_request(Request& request, std::list<Accessory>& accessories) {
  nlohmann::json message;
  std::list<nlohmann::json> accessories_json;
  for(const auto& accessory : accessories) {
    accessories_json.push_back(accessory.serialize());
  }
  message["accessories"] = accessories_json;

  auto body = message.dump();

  request.get_session().send(200, body, "application/hap+json");
}

} // namespace routes
