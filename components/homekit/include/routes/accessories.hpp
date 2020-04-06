#ifndef ROUTES_ACCESSORIES_HPP
#define ROUTES_ACCESSORIES_HPP

#include "i_route.hpp"

namespace routes {

class Accessories : public IRoute {
public:
  void handle_request(Request& request, std::list<Accessory>& accessories);
};

} // namespace routes

#endif // ROUTES_ACCESSORIES_HPP
