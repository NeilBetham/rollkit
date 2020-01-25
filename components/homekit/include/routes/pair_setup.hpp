#ifndef ROUTES_PAIR_SETUP_HPP
#define ROUTES_PAIR_SETUP_HPP

#include "i_route.hpp"
#include "srp.hpp"

namespace routes {


enum class PairSetupStage {
  M1, // Device sends SRP Start message - Exchanges initial SRP params
  M2, // Accessory returns SRP start response - Sends SRP challenge
  M3, // Device sends SRP verify request - Completes challenge
  M4, // Accessory returns verify reponse - Verifies challenge was succesful
  M5, // Device sends Exchange request - Exchanges public keys for encryption
  M6  // Accessory sends Exchange response - Exchanges public keys for encryption
}

class PairSetup : public IRoute {
public:
  PairSetup() {};

private:
  // If this prop is not null then pairing is in progress
  void* _pair_setup_conn;
  PairSetupStage _setup_stage;
};


} // namespace routes

#endif // ROUTES_PAIR_SETUP_HPP
