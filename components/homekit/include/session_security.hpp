#ifndef SESSION_SECURITY_HPP
#define SESSION_SECURITY_HPP

#include <cstdint>
#include <string>

#include "cryptor.hpp"
#include "optional.hpp"

class SessionSecurity {
public:
  SessionSecurity() : _configured(false) {};
  SessionSecurity(const std::string& send_key, const std::string& recv_key) :
    _configured(true), _send_cryptor(send_key, ""), _recv_cryptor(recv_key, "") {};

  std::string encrypt(const std::string& message);
  optional<std::string> decrypt(const std::string& cypher_text);

private:
  bool _configured;
  Cryptor _send_cryptor;
  Cryptor _recv_cryptor;

  uint64_t _recv_count = 0;
  uint64_t _send_count = 0;
};


#endif // SESSION_SECURITY_HPP
