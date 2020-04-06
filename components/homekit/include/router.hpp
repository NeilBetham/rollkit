#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <string>
#include <vector>
#include <tuple>

#include "i_session_delegate.hpp"
#include "i_route.hpp"
#include "request.hpp"


class Router : public ISessionDelegate {
public:
  Router(std::list<Accessory>& accessories_ref) : _accessories(accessories_ref) {};
  ~Router() {};

  virtual void register_route(std::string uri, IRoute* route);
  virtual void request_recv(Request& request);

private:
  std::list<Accessory>& _accessories;
  std::vector<std::tuple<std::string, IRoute*> > routes;
};

#endif // ROUTER_HPP
