#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>
#include <list>
#include <string>
#include <tuple>

namespace internal {

uint8_t hex_c_to_bin_c(char hex);

} // namespace internal

std::string to_hex(const std::string& bin);
std::string to_bin(const std::string& hex);

std::string hap_uuid_prefix(uint32_t value);
std::list<std::tuple<std::string, std::string>> parse_query_string(std::string qs);
list<tuple<uint64_t, uint64_t>> parse_id_list(std::string id_list);

#endif // UTILS_HPP
