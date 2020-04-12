#ifndef ROUTES_PAIR_SETUP_HPP
#define ROUTES_PAIR_SETUP_HPP

#include "srp.hpp"
#include "rollkit/interfaces/i_route.hpp"
#include "rollkit/tlv.hpp"
#include "rollkit/hap_defs.hpp"

namespace rollkit {
namespace routes {


enum class PairState {
  M0, // Idle waiting for pairing request
  M1, // Device exchanges initial SRP params
  M2, // Accessory sends SRP challenge
  M3, // Device completes challenge
  M4, // Accessory verifies challenge was succesful
  M5, // Device exchanges public keys for encryption
  M6  // Accessory exchanges public keys for encryption
};

class PairSetup : public interfaces::IRoute {
public:
  PairSetup() :
    _srp_math(_srp_hash_fn, {HAP_SRP_MODULUS}, {HAP_SRP_GENERATOR}),
    _srp_user(SRP::User::fromPassword(_srp_math, "", "")),
    _srp_verifier(_srp_math, _srp_user)
    {};

  void handle_request(Request& request, interfaces::IApp& app);

private:
  void handle_m1(Request& request, TLVs& tlvs, interfaces::IApp& app);
  void handle_m3(Request& request, TLVs& tlvs);
  void handle_m5(Request& request, TLVs& tlvs);

  void reset() { _pair_in_progress = false; _setup_stage = PairState::M0; };

  bool _pair_in_progress = false;
  PairState _setup_stage;

  SRP::SHA512 _srp_hash_fn;
  SRP::Math _srp_math;
  SRP::User _srp_user;
  SRP::Verifier _srp_verifier;
};


} // namespace routes
} // namespace rollkit
#endif // ROUTES_PAIR_SETUP_HPP
