#ifndef SRP_VERIFIER_HPP
#define SRP_VERIFIER_HPP

#include <string>

#include "srp/bignum.hpp"
#include "srp/math.hpp"
#include "srp/user.hpp"
#include "srp/challenge.hpp"


namespace SRP {


// Handles verification of SRP parameters
class Verifier {
public:
  Verifier(Math& math, const User& user) : _math(math), _user(user) {};

  Challenge get_challenge();
  bool verify(const BigNum& A, const BigNum& M);
  BigNum get_client_proof() { return H_AMK; };
  BigNum get_shared_secret() { return K; }; 

private:
  Math& _math;
  User _user;

  BigNum k;
  BigNum A;
  BigNum B;
  BigNum b;
  BigNum S;
  BigNum K;
  BigNum M;
  BigNum H_AMK;
};


} // namespace SRP

#endif // SRP_VERIFIER_HPP
