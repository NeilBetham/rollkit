#include "srp/utils.hpp"

#include <sodium.h>
#include <string>

#include "esp_log.h"

using namespace std;

namespace SRP {


BigNum mod_pow(const BigNum& b, const BigNum& e, const BigNum& M) {
  return b.mod_pow(e, M);
}


BigNum random(int byte_count) {
  string temp;
  temp.resize(byte_count);

  randombytes_buf((void*)temp.data(), byte_count);

  return BigNum::from_raw(temp);
}


} // namespace SRP
