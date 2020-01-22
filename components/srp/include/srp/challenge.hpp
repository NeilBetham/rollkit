#ifndef SRP_CHALLENGE_HPP
#define SRP_CHALLENGE_HPP

#include "srp/bignum.hpp"
#include "srp/math.hpp"

namespace SRP {


// Holds challenge paramters to send to client
class Challenge {
public:
  Challenge(Math& math, const BigNum& _Be, const BigNum& _salt) :
    _math(math), Be(_Be), salt(_salt) {};

  BigNum get_B() { return Be; };
  BigNum get_salt() { return salt; };

private:
  Math& _math;
  BigNum Be;
  BigNum salt;
};


} // namespace SRP

#endif // SRP_CHALLENGE_HPP
