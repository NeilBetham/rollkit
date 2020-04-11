#ifndef I_ROUTE_HPP
#define I_ROUTE_HPP

#include <list>

#include "accessory_db.hpp"
#include "event_manager.hpp"
#include "i_app.hpp"
#include "request.hpp"

class App;

class IRoute {
public:
  IRoute() {};
  virtual ~IRoute() {};

  virtual void handle_request(Request& request, IApp& app) = 0;
};

#endif // I_ROUTE_HPP
