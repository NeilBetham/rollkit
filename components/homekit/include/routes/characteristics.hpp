#ifndef ROUTES_CHARACTERISTICS_HPP
#define ROUTES_CHARACTERISTICS_HPP

#include "i_route.hpp"

#include "i_route.hpp"

namespace routes {

class Characteristics : public IRoute {
public:
  void handle_request(Request& request, std::list<Accessory>& accessories);
};

} // namespace routes


#endif // ROUTES_CHARACTERISTICS_HPP
