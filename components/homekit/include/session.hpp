#ifndef SESSION_HPP
#define SESSION_HPP

#include <string>

#include "mongoose.h"
#include "i_app_delegate.hpp"
#include "i_session_delegate.hpp"
#include "i_session.hpp"


class Session : public ISession {
public:
  Session();
  ~Session();

  void set_session_delegate(ISessionDelegate* _delegate) { delegate = _delegate; };
  void set_mg_connection(struct mg_connection* _connection) { connection = _connection; }

  void handle_request(std::string& data);

  void head(int code);
  void send(int code, std::string& message);
  void close();
  bool is_closed();

private:
  void handle_message(std::string& data);

  ISessionDelegate* delegate;
  bool is_pair_verified;
  struct mg_connection* connection;
};

#endif // SESSION_HPP
