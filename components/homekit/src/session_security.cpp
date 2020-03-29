#include "session_security.hpp"

#include <esp_log.h>

#include "utils.hpp"

using namespace std;


string SessionSecurity::encrypt(const string& message) {
  if(!_configured) { return ""; }

  uint16_t data_length = message.size();
  string add_data((char*)&data_length, 2);
  _send_cryptor.set_nonce(string((char*)&_send_count, sizeof(_send_count)));
  auto cypher_text = _send_cryptor.encrypt(message, add_data);
  ++_send_count;
  return add_data + cypher_text;
}

optional<string> SessionSecurity::decrypt(const string& cypher_text) {
  if(!_configured) { return {}; }
  string aad(cypher_text.data(), 2);
  string l_cypher_text(cypher_text.data() + aad.size(), cypher_text.size() - aad.size());
  _recv_cryptor.set_nonce(string((char*)&_recv_count, sizeof(_recv_count)));
  auto decrypt_res = _recv_cryptor.decrypt(l_cypher_text, aad);
  ++_recv_count;
  return decrypt_res;
}
