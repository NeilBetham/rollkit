#ifndef CURVE25519_HPP
#define CURVE25519_HPP

#include <string>
#include <tuple>

namespace rollkit {
namespace Curve25519 {


//           Public      Private
std::tuple<std::string, std::string> gen_keypair();
std::string compute_shared_secret(const std::string& private_key, const std::string& public_key);


} // namespace Curve25519
} // namespace rollkit
#endif // CURVE25519_HPP
