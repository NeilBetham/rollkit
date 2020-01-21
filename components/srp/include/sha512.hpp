#ifndef SHA512_HPP
#define SHA512_HPP

#include <sodium.h>

#include "i_hash_function.hpp"

class SHA512 : public IHashFunction {
  string hash(const string& input) {
    string temp(crypto_hash_sha512_BYTES, 0);

    crypto_hash_sha512((unsigned char*)temp.data(), (unsigned char*)input.data(), input.size());

    return move(temp);
  }
};


#endif // SHA512_HPP
