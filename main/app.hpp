#ifndef APP_HPP
#define APP_HPP

#include <string>

#include "mongoose.h"
#include "request.hpp"


class IAppRouteDelegate {
public:
  virtual ~IAppRouteDelegate() = 0;
  virtual void handle_request(Request& req) = 0;
};


class App {
public:
  void handle_event(struct mg_connection *nc, int ev, void *evData);
  void route(std::string route, std::string method, IAppRouteDelegate* delegate);
};

#endif // APP_HPP
