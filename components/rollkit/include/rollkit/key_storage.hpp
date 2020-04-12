#ifndef KEY_STORAGE_HPP
#define KEY_STORAGE_HPP

#include <string>

namespace rollkit {


std::string get_public_key();
std::string get_private_key();
void clear_keys();


} // namespace rollkit
#endif // KEY_STORAGE_HPP
