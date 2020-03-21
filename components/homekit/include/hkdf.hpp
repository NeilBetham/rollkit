#ifndef HKDF_HPP
#define HKDF_HPP

#include <string>
#include <cstdint>

std::string hkdf_sha512(const std::string& key, const std::string& salt, const std::string& info, uint64_t out_size);


#endif // HKDF_HPP
