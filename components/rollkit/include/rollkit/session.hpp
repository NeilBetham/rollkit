#ifndef SESSION_HPP
#define SESSION_HPP

#include <list>
#include <string>

#include "mongoose.h"

#include "rollkit/interfaces/i_session_delegate.hpp"
#include "rollkit/interfaces/i_session.hpp"
#include "rollkit/cryptor.hpp"
#include "rollkit/session_security.hpp"
#include "rollkit/tlv.hpp"

namespace rollkit {


class Session : public interfaces::ISession {
public:
  Session() : _delegate(nullptr), _is_pair_verified(false), _is_admin(false), _connection(nullptr) {};
  Session(interfaces::ISessionDelegate* delegate, struct mg_connection* connection) :
    _delegate(delegate), _is_pair_verified(false), _is_admin(false), _connection(connection) {}
  ~Session() {};

  void handle_request(std::string& data);

  void head(int code);
  void send(int code, const std::string& message, const std::string& content_type);
  void event(const std::string& message);
  void close();
  bool is_closed();
  void* get_identifier() { return (void*)_connection; };
  void setup_security(const std::string& shared_secret, bool is_admin);
  void register_event_listener(interfaces::ISessionEventDelegate* _listener) { _event_listeners.push_back(_listener); };

private:
  void send_data(std::string to_send);
  void handle_message(std::string& data);

  SessionSecurity _session_sec;
  interfaces::ISessionDelegate* _delegate;
  bool _is_pair_verified;
  bool _is_admin;
  std::string _acc_to_cont_key;
  std::string _cont_to_acc_key;
  struct mg_connection* _connection;
  std::list<interfaces::ISessionEventDelegate*> _event_listeners;
};


} // namespace rollkit
#endif // SESSION_HPP
