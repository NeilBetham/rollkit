#ifndef SHA512_HPP
#define SHA512_HPP

#include <mbedtls/sha1.h>

#include "i_hash_function.hpp"

class SHA1 : public IHashFunction {
  SHA1() {
    mbedtls_sha1_init(&_state);
  }

  ~SHA1() {
    mbedtls_sha1_free(&_state);
  }

  string hash(const string& input) {
    string temp(20, 0);
    mbedtls_sha1_starts_ret(&_state);
    mbedtls_sha1_update_ret(&_state, (unsigned char*)input.data(), input.size());
    mbedtls_sha1_finish_ret(&_state, (unsigned char*)(&temp[0]));
    return move(temp);
  }

private:
  mbedtls_sha1_context _state;
};


#endif // SHA512_HPP
