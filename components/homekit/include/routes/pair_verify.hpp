#ifndef ROUTES_PAIR_VERIFY_HPP
#define ROUTES_PAIR_VERIFY_HPP

#include <unordered_map>

#include "i_route.hpp"
#include "tlv.hpp"
#include "hap_defs.hpp"

namespace routes {

enum class VerifyStage {
  M0, // Idle, waiting for verify request
  M1, // Device sends public key
  M2, // Accessory sends encrypted pairing id and signature and plain text pub key
  M3, // Device sends encrypted pairing id and signature
  M4  // Accessory checks received signature in pairings list
};

struct VerifyState {
  VerifyStage stage;
  std::string device_public_key;
  std::string accessory_public_key;
  std::string accessory_private_key;
  std::string session_key;
  std::string shared_secret;
};

class PairVerify : public IRoute {
public:
  void handle_request(Request& request, IApp& app);

private:
  void handle_m1(Request& request, TLVs& tlvs, VerifyState& state);
  void handle_m3(Request& request, TLVs& tlvs, VerifyState& state);

  std::unordered_map<void*, VerifyState> _verify_states;
};

} // namespace routes


#endif // ROUTES_PAIR_VERIFY_HPP
