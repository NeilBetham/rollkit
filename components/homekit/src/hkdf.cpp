#include "hkdf.hpp"

#include <cmath>
#include <list>
#include <sodium.h>

using namespace std;

// See: https://tools.ietf.org/html/rfc5869

namespace {

string expand(const string& salt, const string& key) {
  string prk(crypto_auth_hmacsha512_BYTES, 0);
  crypto_auth_hmacsha512((uint8_t*)&prk[0], (uint8_t*)key.data(), key.size(), (uint8_t*)salt.data());
  return prk;
}

string extract(const string& prk, const string& info, uint64_t out_len) {
  string out;
  string tmp(crypto_auth_hmacsha512_BYTES, 0);
  uint8_t iter_count = 1;

  while(out.size() < out_len) {
    string input;
    if(iter_count != 1) { input += tmp; }
    input += info;
    input += string(1, (char)iter_count);

    crypto_auth_hmacsha512((uint8_t*)&tmp[0], (uint8_t*)&input[0], input.size(), (uint8_t*)prk.data());
    out += tmp;
    iter_count++;
  }

  // Size the output correctly
  out.resize(out_len);

  return out;
}

} // namespace

string hkdf_sha512(const string& key, const string& salt, const string& info, uint64_t out_size) {
  auto prk = expand(salt, key);
  return extract(prk, info, out_size);
}
