#ifndef I_HASH_FUNCTION_HPP
#define I_HASH_FUNCTION_HPP

#include <string>

#include "bignum.hpp"

class IHashFunction {
public:
  ~IHashFunction() {};

  virtual std::string hash(const std::string& input) = 0;
};


#endif // I_HASH_FUNCTION_HPP
