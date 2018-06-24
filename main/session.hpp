#ifndef SESSION_HPP
#define SESSION_HPP

#include <string>

#include "mongoose.h"
#include "i_app_delegate.hpp"
#include "i_session_delegate.hpp"


class Session : public IAppDelegate {
public:
  Session();
  ~Session();

  void set_session_delegate(ISessionDelegate* _delegate) { delegate = _delegate; };

  void handle_request(std::string& data);

  void head(int code);
  void send(int code, std::string message);
  void close();
  bool is_closed();

private:
  ISessionDelegate* delegate;

  struct mg_connection* connection;
};

#endif // SESSION_HPP
