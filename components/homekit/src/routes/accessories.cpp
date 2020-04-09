#include "routes/accessories.hpp"

#include <esp_log.h>

#include "json.hpp"

namespace routes {

void Accessories::handle_request(Request& request, AccessoryDB& acc_db) {
  nlohmann::json message;
  std::list<nlohmann::json> accessories_json;
  for(const auto& accessory : acc_db.get_accessories()) {
    accessories_json.push_back(accessory.serialize());
  }
  message["accessories"] = accessories_json;

  auto body = message.dump();
  ESP_LOGD("rt-accs", "AccessoryDB: %s", body.c_str());
  request.get_session().send(200, body, "application/hap+json");
}

} // namespace routes
