#ifndef I_ROUTE_HPP
#define I_ROUTE_HPP

#include "request.hpp"

class IRoute {
public:
  IRoute() {};
  virtual ~IRoute() {};

  virtual void handle_request(Request& request) = 0;
};

#endif // I_ROUTE_HPP
