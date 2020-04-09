#ifndef ROUTES_CHARACTERISTICS_HPP
#define ROUTES_CHARACTERISTICS_HPP

#include "i_route.hpp"

#include "i_route.hpp"

namespace routes {

class Characteristics : public IRoute {
public:
  void handle_request(Request& request, AccessoryDB& acc_db);

private:
  void handle_get(Request& request, AccessoryDB& acc_db);
  void handle_put(Request& request, AccessoryDB& acc_db);
};

} // namespace routes


#endif // ROUTES_CHARACTERISTICS_HPP
