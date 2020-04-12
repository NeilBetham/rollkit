#ifndef I_ROUTE_HPP
#define I_ROUTE_HPP

#include <list>

#include "rollkit/accessory_db.hpp"
#include "rollkit/event_manager.hpp"
#include "rollkit/interfaces/i_app.hpp"
#include "rollkit/request.hpp"

namespace rollkit {
namespace interfaces {


class IRoute {
public:
  IRoute() {};
  virtual ~IRoute() {};

  virtual void handle_request(Request& request, interfaces::IApp& app) = 0;
};


} // namespace interfaces
} // namespace rollkit
#endif // I_ROUTE_HPP
