#ifndef APP_HPP
#define APP_HPP

#include <string>
#include <unordered_map>

#include "mongoose.h"
#include "session.hpp"
#include "i_app_delegate.hpp"


class App {
public:
  void handle_event(struct mg_connection *nc, int ev, void *ev_data);
  void set_delegate(IAppDelegate* _delegate) { delegate = _delegate; };

private:
  void handle_new_connection(struct mg_connection *nc, void *ev_data);
  void handle_new_data(struct mg_connection *nc, void *ev_data);

  IAppDelegate* delegate;
  std::unordered_map<mg_connection*, IAppDelegate*> sessions;
};

#endif // APP_HPP
