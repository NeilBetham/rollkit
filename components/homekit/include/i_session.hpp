#ifndef I_SESSION_HPP
#define I_SESSION_HPP

#include <string>
#include "i_session_event_delegate.hpp"

class ISession {
public:
  virtual ~ISession() {};

  virtual void head(int code) = 0;
  virtual void send(int code, const std::string& message, const std::string& content_type) = 0;
  virtual void event(const std::string& message) = 0;
  virtual void close() = 0;
  virtual bool is_closed() = 0;
  virtual void* get_identifier() = 0;
  virtual void setup_security(const std::string& shared_secret, bool is_admin) = 0;
  virtual void register_event_listener(ISessionEventDelegate* _listener) = 0;
};


#endif // I_SESSION_HPP
