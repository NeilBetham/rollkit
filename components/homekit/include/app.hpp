#ifndef APP_HPP
#define APP_HPP

#include <list>
#include <string>
#include <unordered_map>

#include <esp_log.h>

#include "mongoose.h"
#include "router.hpp"
#include "routes/pair_setup.hpp"
#include "routes/pair_verify.hpp"
#include "routes/accessories.hpp"
#include "session_manager.hpp"


class App {
public:
  App() : _router(_accessories), _session_manager(&_router) {
    _router.register_route("/pair-setup", &_ps_route);
    _router.register_route("/pair-verify", &_pv_route);
    _router.register_route("/accessories", &_accs_route);
  };

  void init(std::string name, std::string model, std::string manu, std::string firmware_rev);
  void register_accessory(const Accessory& acc) {
    _accessories.push_back(acc);
    _accessories.back().register_service(_accessory_info);
  }
  void handle_mg_event(struct mg_connection *nc, int event, void *event_data) {
    _session_manager.handle_mg_event(nc, event, event_data);
  }

private:
  std::list<Accessory> _accessories;
  Service _accessory_info;
  routes::PairSetup _ps_route;
  routes::PairVerify _pv_route;
  routes::Accessories _accs_route;

  Router _router;
  SessionManager _session_manager;
};

#endif // APP_HPP
