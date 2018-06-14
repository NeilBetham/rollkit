#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <string>
#include <vector>
#include <tuple>

#include "request.hpp"
#include "session.hpp"

class IRoute {
public:
  IRoute() {};
  virtual ~IRoute() {};

  virtual void handle_request(Request& request, Session& session) = 0;
};

class Router {
public:
  Router() {};
  ~Router() {};

  void register_route(std::string uri, IRoute* route);
  void route_request(Request& request, Session& session);

private:
  std::vector<std::tuple<std::string, IRoute*> > routes;
};

#endif // ROUTER_HPP
