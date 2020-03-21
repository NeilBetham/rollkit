#ifndef CRYPTOR_HPP
#define CRYPTOR_HPP

#include <string>

#include "optional.hpp"

class Cryptor {
public:
  Cryptor(const std::string& key, const std::string& nonce);

  std::string encrypt(const std::string& plain_text);
  optional<std::string> decrypt(const std::string& cypher_text);
  void set_nonce(const std::string& nonce);

private:
  void pad_and_set_nonce(const std::string& nonce);

  std::string _key;
  std::string _nonce;
};


#endif // CRYPTOR_HPP
