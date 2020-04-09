#include "routes/characteristics.hpp"

#include <list>
#include <esp_log.h>

#include "utils.hpp"

using namespace std;

namespace routes {

void Characteristics::handle_request(Request& request, AccessoryDB& acc_db) {
  if(request.get_method() == "GET") {
    handle_get(request, acc_db);
  } else if(request.get_method() == "PUT") {
    handle_put(request, acc_db);
  }
};

void Characteristics::handle_get(Request& request, AccessoryDB& acc_db) {
  auto query_info = parse_query_string(request.get_query_string());
  auto id_list = parse_id_list(query_info["id"]);

  list<Characteristic> selected_chars;

  for(auto& id : id_list) {
    uint64_t acc_id;
    uint64_t char_id;
    tie(acc_id, char_id) = id;

    auto char_opt = acc_db.find_char(acc_id, char_id);
    if(char_opt) {
      selected_chars.push_back(*char_opt);
    }
  }

  // Build json response
  list<nlohmann::json> char_json;
  for(auto& characteristic : selected_chars) {
    char_json.push_back(characteristic.serialize());
  }
  nlohmann::json resp;
  resp["characteristics"] = char_json;

  // send it
  auto body = resp.dump();

  ESP_LOGD("rt-chars", "Sending: %s", body.c_str());
  request.get_session().send(200, body, "application/hap+json");
};

void Characteristics::handle_put(Request& request, AccessoryDB& acc_db) {
  nlohmann::json char_write_req = nlohmann::json::parse(request.get_body());

  auto char_write_reqs = char_write_req["characteristics"];
  if(char_write_reqs.is_null()) { return; }

  for(auto& req : char_write_reqs) {
    uint64_t acc_id = req["aid"];
    uint64_t char_id = req["iid"];
    auto value = req["value"];

    auto char_opt = acc_db.find_char(acc_id, char_id);
    if(!char_opt) { continue; }

    char_opt->handle_update(value);
  }

  request.get_session().head(204);
}


} // namespace routes
