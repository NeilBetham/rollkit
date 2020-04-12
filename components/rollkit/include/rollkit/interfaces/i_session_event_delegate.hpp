#ifndef I_SESSION_EVENT_DELEGATE_HPP
#define I_SESSION_EVENT_DELEGATE_HPP

namespace rollkit {
namespace interfaces {


class ISessionEventDelegate {
public:
  ~ISessionEventDelegate() {};

  virtual void session_closed(void* session_id) = 0;
};


} // namespace interfaces
} // namespace rollkit
#endif // I_SESSION_EVENT_DELEGATE_HPP
