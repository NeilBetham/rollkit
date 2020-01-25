#ifndef APP_HPP
#define APP_HPP

#include <string>
#include <unordered_map>

#include <esp_log.h>

#include "mongoose.h"
#include "router.hpp"
#include "session_manager.hpp"


class App {
public:
  App() : _session_manager(&_router) {};

  void handle_mg_event(struct mg_connection *nc, int event, void *event_data) {
    _session_manager.handle_mg_event(nc, event, event_data);
  }

private:
  Router _router;
  SessionManager _session_manager;
};

#endif // APP_HPP
