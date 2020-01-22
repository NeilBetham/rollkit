#ifndef SRP_UTILS_HPP
#define SRP_UTILS_HPP

#include "srp/bignum.hpp"

namespace SRP {

// Shim for BigNum modpow function
BigNum mod_pow(const BigNum& b, const BigNum& e, const BigNum& M);

// Interface to external libs
BigNum random(int byte_count);


} // namespace SRP


#endif // SRP_UTILS_HPP
