#ifndef ED25519_HPP
#define ED25519_HPP

#include <string>

bool verify_ed25519(const std::string& signature, const std::string& message, const std::string& public_key);
std::string sign_ed25519(const std::string& message, const std::string& private_key);


#endif // ED25519_HPP
