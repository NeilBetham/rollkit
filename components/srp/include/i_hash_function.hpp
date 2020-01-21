#ifndef I_HASH_FUNCTION_HPP
#define I_HASH_FUNCTION_HPP

#include "bignum.hpp"

class IHashFunction {
  ~IHashFunction() {};

  virtual string hash(const string& input) = 0;
};


#endif I_HASH_FUNCTION_HPP
