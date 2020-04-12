#include "rollkit/ed25519.hpp"

#include <sodium.h>

using namespace std;

namespace rollkit {


bool verify_ed25519(const string& signature, const string& message, const string& public_key) {
  int res = crypto_sign_verify_detached(
    (uint8_t*)signature.data(),
    (uint8_t*)message.data(),
    message.size(),
    (uint8_t*)public_key.data()
  );

  if(res == 0) {
    return true;
  } else {
    return false;
  }
}

string sign_ed25519(const string& message, const string& private_key) {
  string signature(crypto_sign_BYTES, 0);

  crypto_sign_detached(
    (uint8_t*)&signature[0],
    NULL,
    (uint8_t*)message.data(),
    message.size(),
    (uint8_t*)private_key.data()
  );

  return signature;
}


} // namespace rollkit
