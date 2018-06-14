#ifndef SESSION_HPP
#define SESSION_HPP

#include <string>

#include "mongoose.h"
#include "request.hpp"

class Session;

class ISessionDelegate {
public:
  ISessionDelegate() {};
  virtual ~ISessionDelegate() {};

  virtual void request_recv(Request& new_request, Session& session) = 0;
};


class Session {
public:
  Session();
  ~Session();

  void set_session_delegate();

  void head(int code);
  void send(int code, std::string message);
  void close();
  bool is_closed();

private:
  struct mg_connection* connection;
};

#endif // SESSION_HPP
