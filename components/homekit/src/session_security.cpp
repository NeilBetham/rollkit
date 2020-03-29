#include "session_security.hpp"

using namespace std;


string SessionSecurity::encrypt(const string& message) {
  if(!_configured) { return ""; }

  uint16_t data_length = message.size();
  string add_data((char*)&data_length, 2);
  _send_cryptor.set_nonce(to_string(_send_count));
  auto cypher_text = _send_cryptor.encrypt(message, add_data);
  ++_send_count;
  return cypher_text;
}

optional<string> SessionSecurity::decrypt(const string& cypher_text) {
  if(!_configured) { return {}; }
  uint16_t data_length = (cypher_text[0] << 8) | cypher_text[1];
  _recv_cryptor.set_nonce(to_string(_recv_count));
  auto decrypt_res = _recv_cryptor.decrypt(cypher_text.substr(2, data_length + 16), true);
  ++_recv_count;
  return decrypt_res;
}
