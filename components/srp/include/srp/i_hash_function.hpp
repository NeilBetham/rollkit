#ifndef SRP_I_HASH_FUNCTION_HPP
#define SRP_I_HASH_FUNCTION_HPP

#include <string>

#include "bignum.hpp"

class IHashFunction {
public:
  ~IHashFunction() {};

  virtual std::string hash(const std::string& input) = 0;
};


#endif // SRP_I_HASH_FUNCTION_HPP
