#ifndef ROUTES_PAIRINGS_HPP
#define ROUTES_PAIRINGS_HPP

#include "i_route.hpp"
#include "tlv.hpp"
#include "hap_defs.hpp"

namespace routes {


class Pairings : public IRoute {
public:
  void handle_request(Request& request, AccessoryDB& acc_db, EventManager& ev_mgr);

private:
  void handle_add_pairing(Request& request, TLVs& tlvs);
  void handle_remove_pairing(Request& request, TLVs& tlvs);
  void handle_list_pairing(Request& request, TLVs& tlvs);
};


} // namespace routes


#endif // ROUTES_PAIRINGS_HPP
