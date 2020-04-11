#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <string>
#include <vector>
#include <tuple>

#include "accessory_db.hpp"
#include "event_manager.hpp"
#include "i_session_delegate.hpp"
#include "i_route.hpp"
#include "request.hpp"

class App;

class Router : public ISessionDelegate {
public:
  Router(IApp& app) : _app(app) {};
  ~Router() {};

  virtual void register_route(std::string uri, IRoute* route);
  virtual void request_recv(Request& request);

private:
  IApp& _app;
  std::vector<std::tuple<std::string, IRoute*> > routes;
};

#endif // ROUTER_HPP
