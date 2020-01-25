#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include <unordered_map>

#include "mongoose.h"
#include "session.hpp"
#include "i_session_delegate.hpp"

class SessionManager {
public:
  SessionManager(ISessionDelegate* delegate);
  ~SessionManager();

  void handle_mg_event(struct mg_connection* nc, int event, void* event_data);

private:
  ISessionDelegate* _session_delegate;
  std::unordered_map<void*, Session> sessions;
};


#endif // SESSION_MANAGER_HPP
