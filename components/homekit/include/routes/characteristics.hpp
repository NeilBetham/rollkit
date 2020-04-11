#ifndef ROUTES_CHARACTERISTICS_HPP
#define ROUTES_CHARACTERISTICS_HPP

#include "i_route.hpp"

namespace routes {

class Characteristics : public IRoute {
public:
  void handle_request(Request& request, IApp& app);

private:
  void handle_get(Request& request, IApp& app);
  void handle_put(Request& request, IApp& app);
};

} // namespace routes


#endif // ROUTES_CHARACTERISTICS_HPP
