#ifndef HKDF_HPP
#define HKDF_HPP

#include <string>
#include <cstdint>

namespace rollkit {
namespace internal {

std::string hmac_sha512(const std::string& key, const std::string& message);
std::string extract(const std::string& salt, const std::string& key);
std::string expand(const std::string& prk, const std::string& info, uint64_t out_len);

} // namespace internal

std::string hkdf_sha512(const std::string& key, const std::string& salt, const std::string& info, uint64_t out_size);

} // namespace rollkit
#endif // HKDF_HPP
