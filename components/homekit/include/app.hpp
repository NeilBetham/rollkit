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
#include "routes/characteristics.hpp"
#include "routes/pairings.hpp"
#include "session_manager.hpp"
#include "accessory_db.hpp"
#include "mdns.hpp"
#include "event_manager.hpp"


class App {
public:
  App() : _event_mgr(_accessory_db), _router(_accessory_db, _event_mgr), _session_manager(&_router) {
    _router.register_route("/pair-setup", &_ps_route);
    _router.register_route("/pair-verify", &_pv_route);
    _router.register_route("/accessories", &_accs_route);
    _router.register_route("/characteristics", &_chars_route);
    _router.register_route("/pairings", &_pairings_route);
  };

  void init(std::string name, std::string model, std::string manu, std::string firmware_rev);
  void register_accessory(const Accessory& acc) {
    Accessory copy_acc = Accessory(acc);
    copy_acc.register_service(_accessory_info);
    copy_acc.register_service(_protocol_info);
    _accessory_db.register_accessory(copy_acc);
  }
  void handle_mg_event(struct mg_connection *nc, int event, void *event_data) {
    _session_manager.handle_mg_event(nc, event, event_data);
  }

private:
  AccessoryDB _accessory_db;
  EventManager _event_mgr;
  Service _accessory_info;
  Service _protocol_info;
  routes::PairSetup _ps_route;
  routes::PairVerify _pv_route;
  routes::Accessories _accs_route;
  routes::Characteristics _chars_route;
  routes::Pairings _pairings_route;

  Router _router;
  SessionManager _session_manager;
  MDNSManager _mdns_mgr;
};

#endif // APP_HPP
