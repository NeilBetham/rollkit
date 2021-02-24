#ifndef APP_HPP
#define APP_HPP

#include <list>
#include <string>
#include <unordered_map>

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

#include "mongoose.h"

#include "rollkit/interfaces/i_app.hpp"
#include "rollkit/accessory_db.hpp"
#include "rollkit/event_manager.hpp"
#include "rollkit/mdns.hpp"
#include "rollkit/router.hpp"
#include "rollkit/routes/pair_setup.hpp"
#include "rollkit/routes/pair_verify.hpp"
#include "rollkit/routes/accessories.hpp"
#include "rollkit/routes/characteristics.hpp"
#include "rollkit/routes/pairings.hpp"
#include "rollkit/session_manager.hpp"

namespace rollkit {


class App : public interfaces::IApp {
public:
  App() : _event_mgr(_accessory_db), _router(*this), _session_manager(&_router) {
    _router.register_route("/pair-setup", &_ps_route);
    _router.register_route("/pair-verify", &_pv_route);
    _router.register_route("/accessories", &_accs_route);
    _router.register_route("/characteristics", &_chars_route);
    _router.register_route("/pairings", &_pairings_route);
  };

  void init(std::string name, std::string model, std::string manu, std::string firmware_rev, std::string setup_code, std::string mac);
  void start();
  void stop();
  void register_accessory(const Accessory& acc) {
    Accessory copy_acc = Accessory(acc);
    copy_acc.register_service(_accessory_info);
    copy_acc.register_service(_protocol_info);
    _accessory_db.register_accessory(copy_acc);
  }
  void handle_mg_event(struct mg_connection *nc, int event, void *event_data) {
    _session_manager.handle_mg_event(nc, event, event_data);
  }
  AccessoryDB& get_acc_db() { return _accessory_db; };
  EventManager& get_ev_mgr() { return _event_mgr; };
  std::string get_setup_code() { return _setup_code; };

  void run();

private:
  std::string _setup_code;

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

  // FreeRTOS State to manage task execution
  struct mg_mgr _mg_mgr;
  TaskHandle_t _main_task;
};


} // namespace rollkit
#endif // APP_HPP
