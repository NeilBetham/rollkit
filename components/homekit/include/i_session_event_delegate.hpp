#ifndef I_SESSION_EVENT_DELEGATE_HPP
#define I_SESSION_EVENT_DELEGATE_HPP


class ISessionEventDelegate {
public:
  ~ISessionEventDelegate() {};

  virtual void session_closed(void* session_id) = 0;
};


#endif // I_SESSION_EVENT_DELEGATE_HPP
