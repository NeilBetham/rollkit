#ifndef SHA512_HPP
#define SHA512_HPP

#include <string>
#include <sodium.h>

#include "i_hash_function.hpp"

class SHA512 : public IHashFunction {
public:
  std::string hash(const std::string& input) {
    std::string temp(crypto_hash_sha512_BYTES, 0);

    crypto_hash_sha512((unsigned char*)temp.data(), (unsigned char*)input.data(), input.size());

    return move(temp);
  }
};


#endif // SHA512_HPP
