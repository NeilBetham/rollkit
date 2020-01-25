#include <string>
#include <cstring>
#include <utility>
#include <functional>

#include "session_manager.hpp"
#include "request.hpp"

using namespace std;

SessionManager::SessionManager(ISessionDelegate* delegate) : _session_delegate(delegate) {
}

SessionManager::~SessionManager(){
  for(auto &session : sessions){
    session.second.close();
  }
}

void SessionManager::handle_mg_event(struct mg_connection* nc, int event, void *event_data){
  switch(event){
    // New connection
    case MG_EV_ACCEPT: {
      sessions.emplace(std::piecewise_construct, std::make_tuple(nc), std::make_tuple(_session_delegate, nc));
      break;
    }

    // New data on existing connection
    case MG_EV_RECV: {
      int recevied_bytes = nc->recv_mbuf.len;

      // Allocate a string to fit the recevied data
      string message;
      message.resize(recevied_bytes);

      // Copy recived contents to local buffer
      memcpy(&message[0], nc->recv_mbuf.buf, recevied_bytes);

      // Clear mongoose buffer
      mbuf_remove(&nc->recv_mbuf, recevied_bytes);

      // Give data to session for processing
      sessions[nc].handle_request(message);
      break;
    }

    // Existing connection closed
    case MG_EV_CLOSE: {
      if(sessions.find(nc) != sessions.end()){
        // If the session exists close it
        sessions[nc].close();
        sessions.erase(nc);
      }
      break;
    }
    default:
      break;
  }
}
