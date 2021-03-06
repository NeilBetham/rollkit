#include "rollkit/routes/characteristics.hpp"

#include <list>
#include <esp_log.h>

#include "rollkit/utils.hpp"

using namespace std;

namespace rollkit {
namespace routes {


void Characteristics::handle_request(Request& request, interfaces::IApp& app) {
  if(request.get_method() == "GET") {
    handle_get(request, app);
  } else if(request.get_method() == "PUT") {
    handle_put(request, app);
  }
};

void Characteristics::handle_get(Request& request, interfaces::IApp& app) {
  auto query_info = parse_query_string(request.get_query_string());
  auto id_list = parse_id_list(query_info["id"]);
  ESP_LOGD("rt-chars", "Query String: %s", request.get_query_string().c_str());
  ESP_LOGD("rt-chars", "ID String: %s", query_info["id"].c_str());
  nlohmann::json resp;

  for(auto& id : id_list) {
    uint64_t acc_id;
    uint64_t char_id;
    tie(acc_id, char_id) = id;

    ESP_LOGD("rt-chars", "Looking for char: %llu", char_id);
    auto char_opt = app.get_acc_db().find_char(acc_id, char_id);
    if(char_opt) {
      ESP_LOGD("rt-chars", "Found char: %llu", char_id);
      resp["characteristics"].push_back({
        {"aid", acc_id},
        {"iid", char_id},
        {"value", char_opt->handle_read()}
      });
    }
  }

  // send it
  auto body = resp.dump();

  ESP_LOGD("rt-chars", "Sending: %s", body.c_str());
  request.get_session().send(200, body, "application/hap+json");
};

void Characteristics::handle_put(Request& request, interfaces::IApp& app) {
  nlohmann::json char_write_req = nlohmann::json::parse(request.get_body());
  ESP_LOGD("rt-chars", "PUT request body: %s", request.get_body().c_str());

  auto char_write_reqs = char_write_req["characteristics"];
  if(char_write_reqs.is_null()) { return; }

  for(auto& req : char_write_reqs) {
    uint64_t acc_id = req["aid"];
    uint64_t char_id = req["iid"];
    if(req.contains("ev")) {
      // Handle event sub request
      if(req["ev"] == true) {
        ESP_LOGD("rt-chars", "Event Request: %s", req.dump().c_str());
        ESP_LOGD("rt-chars", "Controller Registering For Events");
        app.get_ev_mgr().register_for_events(&request.get_session(), char_id);
        request.get_session().register_event_listener(&app.get_ev_mgr());
      } else {
        ESP_LOGD("rt-chars", "Controller Unregistering For Events");
        app.get_ev_mgr().unregister_for_events(&request.get_session(), char_id);
      }
    } else {
      // Handle char put request
      auto value = req["value"];

      auto char_opt = app.get_acc_db().find_char(acc_id, char_id);
      if(!char_opt) { continue; }

      char_opt->handle_update(value);
      app.get_ev_mgr().characteristic_updated(acc_id, char_id);
    }
  }

  request.get_session().head(204);
}


} // namespace routes
} // namespace rollkit
