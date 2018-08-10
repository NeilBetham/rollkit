#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include <unordered_map>

#include "mongoose.h"
#include "session.hpp"

class SessionManager {
public:
  SessionManager();
  ~SessionManager();

  void handle_mg_event(struct mg_connection* nc, int event, void* event_data);

private:

  std::unordered_map<void*, Session> sessions;
};


#endif // SESSION_MANAGER_HPP
