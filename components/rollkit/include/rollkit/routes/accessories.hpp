#ifndef ROUTES_ACCESSORIES_HPP
#define ROUTES_ACCESSORIES_HPP

#include "rollkit/interfaces/i_route.hpp"

namespace rollkit {
namespace routes {


class Accessories : public interfaces::IRoute {
public:
  void handle_request(Request& request, interfaces::IApp& app);
};


} // namespace routes
} // namespace rollkit
#endif // ROUTES_ACCESSORIES_HPP
