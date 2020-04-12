#ifndef ROUTES_PAIRINGS_HPP
#define ROUTES_PAIRINGS_HPP

#include "rollkit/interfaces/i_route.hpp"
#include "rollkit/tlv.hpp"
#include "rollkit/hap_defs.hpp"

namespace rollkit {
namespace routes {


class Pairings : public interfaces::IRoute {
public:
  void handle_request(Request& request, interfaces::IApp& app);

private:
  void handle_add_pairing(Request& request, TLVs& tlvs);
  void handle_remove_pairing(Request& request, TLVs& tlvs);
  void handle_list_pairing(Request& request, TLVs& tlvs);
};


} // namespace routes
} // namespace rollkit
#endif // ROUTES_PAIRINGS_HPP
