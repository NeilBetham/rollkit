#include "srp.hpp"

using namespace std;

namespace SRP {


// class User
User User::fromPassword(Math& math, const std::string &username, const std::string &password) {
  BigNum rand_salt = random(32);
  BigNum x = math.get_x(username, password, rand_salt);
  BigNum v = math.get_v(x);

  return User(math, username, v, rand_salt);
}


// class Verifier
Challenge Verifier::get_challenge(const BigNum& _A) {
  if((A % _math.N()) == 0) {
    A = _A;
  }

  k = _math.get_k();
  b = random(32);
  B = _math.get_B(b, k, _user.get_verifier());
  return Challenge(_math, B, _user.get_salt());
}


bool Verifier::verify(const BigNum& client_M) {
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
