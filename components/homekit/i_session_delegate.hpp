#ifndef I_SESSION_H_DELEGATE_HPP
#define I_SESSION_H_DELEGATE_HPP

#include "request.hpp"
#include "i_session.hpp"


class ISessionDelegate {
public:
  ISessionDelegate() {};
  virtual ~ISessionDelegate() {};

  virtual void request_recv(Request& new_request, ISession* session) = 0;
};

#endif // I_SESSION_DELEGATE_HPP
