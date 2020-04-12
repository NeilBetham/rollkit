#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include <unordered_map>

#include "mongoose.h"

#include "rollkit/interfaces/i_session_delegate.hpp"
#include "rollkit/session.hpp"

namespace rollkit {


class SessionManager {
public:
  SessionManager(interfaces::ISessionDelegate* delegate);
  ~SessionManager();

  void handle_mg_event(struct mg_connection* nc, int event, void* event_data);

private:
  interfaces::ISessionDelegate* _session_delegate;
  std::unordered_map<void*, Session> sessions;
};


} // namespace rollkit
#endif // SESSION_MANAGER_HPP
