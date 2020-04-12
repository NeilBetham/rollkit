#ifndef I_SESSION_H_DELEGATE_HPP
#define I_SESSION_H_DELEGATE_HPP

#include "rollkit/request.hpp"
#include "rollkit/interfaces/i_session.hpp"

namespace rollkit {
namespace interfaces {


class ISessionDelegate {
public:
  ISessionDelegate() {};
  virtual ~ISessionDelegate() {};

  virtual void request_recv(Request& new_request) = 0;
};


} // namespace interfaces
} // namespace rollkit
#endif // I_SESSION_DELEGATE_HPP
