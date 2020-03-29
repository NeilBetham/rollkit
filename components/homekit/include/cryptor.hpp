#ifndef CRYPTOR_HPP
#define CRYPTOR_HPP

#include <string>
#include <tuple>

#include "optional.hpp"

class Cryptor {
public:
  Cryptor() {};
  Cryptor(const std::string& key, const std::string& nonce);

  std::string encrypt(const std::string& plain_text);
  std::string encrypt(const std::string& plain_text, const std::string& add_data);

  optional<std::string> decrypt(const std::string& cypher_text);
  optional<std::string> decrypt(const std::string& cypher_text, bool with_aad);

  void set_nonce(const std::string& nonce);

private:
  void pad_and_set_nonce(const std::string& nonce);

  std::string _key;
  std::string _nonce;
};


#endif // CRYPTOR_HPP
