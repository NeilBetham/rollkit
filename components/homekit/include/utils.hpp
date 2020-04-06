#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>
#include <string>

namespace internal {

uint8_t hex_c_to_bin_c(char hex);

} // namespace internal

std::string to_hex(const std::string& bin);
std::string to_bin(const std::string& hex);

std::string hap_uuid_prefix(uint32_t value);

#endif // UTILS_HPP
