#include "rollkit/curve25519.hpp"

#include <sodium.h>
#include <stdexcept>

namespace rollkit {
namespace Curve25519 {


std::tuple<std::string, std::string> gen_keypair() {
  std::string public_key(crypto_kx_PUBLICKEYBYTES, 0);
  std::string private_key(crypto_kx_SECRETKEYBYTES, 0);

  crypto_kx_keypair((uint8_t*)&public_key[0], (uint8_t*)&private_key[0]);

  return {public_key, private_key};
}

std::string compute_shared_secret(const std::string& private_key, const std::string& public_key) {
  std::string shared_secret(crypto_scalarmult_BYTES, 0);

  if(crypto_scalarmult((uint8_t*)&shared_secret[0], (uint8_t*)private_key.data(), (uint8_t*)public_key.data()) != 0) {
    throw std::runtime_error("Curve25519: Failed to generate shared secret");
  }

  return shared_secret;
}


} // namespace Curve25519
} // namespace rollkit
