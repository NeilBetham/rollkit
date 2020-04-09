#ifndef I_ROUTE_HPP
#define I_ROUTE_HPP

#include <list>

#include "accessory_db.hpp"
#include "request.hpp"

class IRoute {
public:
  IRoute() {};
  virtual ~IRoute() {};

  virtual void handle_request(Request& request, AccessoryDB& acc_db) = 0;
};

#endif // I_ROUTE_HPP
