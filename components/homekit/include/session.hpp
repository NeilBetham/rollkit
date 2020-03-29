#ifndef SESSION_HPP
#define SESSION_HPP

#include <string>

#include "cryptor.hpp"
#include "i_session_delegate.hpp"
#include "i_session.hpp"
#include "mongoose.h"
#include "session_security.hpp"
#include "tlv.hpp"


class Session : public ISession {
public:
  Session() : _delegate(nullptr), _is_pair_verified(false), _is_admin(false), _connection(nullptr) {};
  Session(ISessionDelegate* delegate, struct mg_connection* connection) :
    _delegate(delegate), _is_pair_verified(false), _is_admin(false), _connection(connection) {}
  ~Session() {};

  void handle_request(std::string& data);

  void head(int code);
  void send(int code, const std::string& message, const std::string& content_type);
  void close();
  bool is_closed();
  void* get_identifier() { return (void*)_connection; };
  void setup_security(const std::string& shared_secret, bool is_admin);

private:
  void handle_message(std::string& data);

  SessionSecurity _session_sec;
  ISessionDelegate* _delegate;
  bool _is_pair_verified;
  bool _is_admin;
  std::string _acc_to_cont_key;
  std::string _cont_to_acc_key;
  struct mg_connection* _connection;
};

#endif // SESSION_HPP
