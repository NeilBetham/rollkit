#ifndef SRP_HASH_FN_SHA512_HPP
#define SRP_HASH_FN_SHA512_HPP

#include <string>
#include <sodium.h>

#include "srp/i_hash_function.hpp"

namespace SRP {


class SHA512 : public IHashFunction {
public:
  std::string hash(const std::string& input) {
    std::string temp(crypto_hash_sha512_BYTES, 0);

    crypto_hash_sha512((unsigned char*)temp.data(), (unsigned char*)input.data(), input.size());

    return move(temp);
  }
};


} // namespace SRP

#endif // SRP_HASH_FN_SHA512_HPP
