#include "srp/verifier.hpp"

#include "srp/utils.hpp"

namespace SRP {


Challenge Verifier::get_challenge() {
  k = _math.get_k();
  b = random(32);
  B = _math.get_B(b, k, _user.get_verifier());
  return Challenge(_math, B, _user.get_salt());
}

bool Verifier::verify(const BigNum& _A, const BigNum& client_M) {
  A = _A;
  bool verified = false;
  BigNum u = _math.hash({A, B});
  S = _math.get_S_host(A, _user.get_verifier(), u, b);
  K = _math.get_K(S);
  M = _math.get_M_client(_user.get_username(), _user.get_salt(), A, B, K);

  if(M == client_M) {
    verified = true;
  }

  H_AMK = _math.get_M_host(A, M, K);
  return verified;
}


} // namespace SRP
