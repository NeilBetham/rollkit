#ifndef I_SESSION_HPP
#define I_SESSION_HPP

#include <string>

class ISession {
public:
  virtual ~ISession() {};

  virtual void head(int code) = 0;
  virtual void send(int code, std::string& message) = 0;
  virtual void close() = 0;
  virtual bool is_closed() = 0;
};


#endif // I_SESSION_HPP
