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


class Router : public ISessionDelegate {
public:
  Router(AccessoryDB& acc_db, EventManager& ev_mgr) : _acc_db(acc_db), _ev_mgr(ev_mgr) {};
  ~Router() {};

  virtual void register_route(std::string uri, IRoute* route);
  virtual void request_recv(Request& request);

private:
  AccessoryDB& _acc_db;
  EventManager& _ev_mgr;
  std::vector<std::tuple<std::string, IRoute*> > routes;
};

#endif // ROUTER_HPP
