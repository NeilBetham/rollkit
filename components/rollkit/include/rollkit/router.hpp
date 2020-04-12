#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <string>
#include <vector>
#include <tuple>

#include "rollkit/interfaces/i_route.hpp"
#include "rollkit/interfaces/i_session_delegate.hpp"
#include "rollkit/accessory_db.hpp"
#include "rollkit/event_manager.hpp"
#include "rollkit/request.hpp"

namespace rollkit {


class Router : public interfaces::ISessionDelegate {
public:
  Router(interfaces::IApp& app) : _app(app) {};
  ~Router() {};

  virtual void register_route(std::string uri, interfaces::IRoute* route);
  virtual void request_recv(Request& request);

private:
  interfaces::IApp& _app;
  std::vector<std::tuple<std::string, interfaces::IRoute*> > routes;
};


} // namespace rollkit
#endif // ROUTER_HPP
