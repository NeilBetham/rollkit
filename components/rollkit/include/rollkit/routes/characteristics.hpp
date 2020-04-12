#ifndef ROUTES_CHARACTERISTICS_HPP
#define ROUTES_CHARACTERISTICS_HPP

#include "rollkit/interfaces/i_route.hpp"

namespace rollkit {
namespace routes {


class Characteristics : public interfaces::IRoute {
public:
  void handle_request(Request& request, interfaces::IApp& app);

private:
  void handle_get(Request& request, interfaces::IApp& app);
  void handle_put(Request& request, interfaces::IApp& app);
};


} // namespace routes
} // namespace rollkit
#endif // ROUTES_CHARACTERISTICS_HPP
