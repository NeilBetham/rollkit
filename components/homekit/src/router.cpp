#include "router.hpp"

#include <esp_log.h>

using namespace std;

void Router::register_route(std::string uri, IRoute* route) {
  routes.push_back(
    make_tuple(
      uri,
      route
    )
  );
}


void Router::request_recv(Request& request) {
  ESP_LOGI("router", "New Request: %s - %s", request.get_uri().c_str(), request.get_method().c_str());
  for(auto &route_tuple : routes){
    if(get<0>(route_tuple).compare(request.get_uri()) == 0){
      get<1>(route_tuple)->handle_request(request, _acc_db, _ev_mgr);
    }
  }

  _ev_mgr.flush();
}
