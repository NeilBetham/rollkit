#include "hkdf.hpp"

#include <cmath>
#include <list>
#include <sodium.h>

using namespace std;

// See: https://tools.ietf.org/html/rfc5869

namespace internal {

string hmac_sha512(const string& key, const string& message) {
  string hmac_out(crypto_auth_hmacsha512_BYTES, 0);
  crypto_auth_hmacsha512_state state;

  crypto_auth_hmacsha512_init(&state, (uint8_t*)key.data(), key.size());
  crypto_auth_hmacsha512_update(&state, (uint8_t*)message.data(), message.size());
  crypto_auth_hmacsha512_final(&state, (uint8_t*)&hmac_out[0]);

  return hmac_out;
}

string extract(const string& salt, const string& key) {
  return hmac_sha512(salt, key);
}

string expand(const string& prk, const string& info, uint64_t out_len) {
  string out;
  string tmp;
  uint8_t iter_count = 1;

  while(out.size() < out_len) {
    string input = tmp + info + string(1, (uint8_t)iter_count);
    tmp = hmac_sha512(prk, input);
    out += tmp;
    iter_count++;
  }

  // Size the output correctly
  out.resize(out_len);

  return out;
}

} // internal namespace

string hkdf_sha512(const string& key, const string& salt, const string& info, uint64_t out_size) {
  auto prk = internal::extract(salt, key);
  return internal::expand(prk, info, out_size);
}
