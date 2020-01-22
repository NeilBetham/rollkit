#ifndef SRP_HASH_FN_SHA1_HPP
#define SRP_HASH_FN_SHA1_HPP

#include <mbedtls/sha1.h>
#include <string>

#include "srp/i_hash_function.hpp"

namespace SRP {


class SHA1 : public IHashFunction {
public:
  SHA1() {
    mbedtls_sha1_init(&_state);
  }

  ~SHA1() {
    mbedtls_sha1_free(&_state);
  }

  std::string hash(const std::string& input) {
    std::string temp(20, 0);
    mbedtls_sha1_starts_ret(&_state);
    mbedtls_sha1_update_ret(&_state, (unsigned char*)input.data(), input.size());
    mbedtls_sha1_finish_ret(&_state, (unsigned char*)(&temp[0]));
    return move(temp);
  }

private:
  mbedtls_sha1_context _state;
};


} // namespace SRP

#endif // SRP_HASH_FN_SHA1_HPP
