#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

namespace internal {

uint8_t hex_c_to_bin_c(char hex);

} // namespace internal

std::string to_hex(const std::string& bin);
std::string to_bin(const std::string& hex);

#endif // UTILS_HPP
