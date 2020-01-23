#include <string>
#include <cstring>

#include "session_manager.hpp"
#include "request.hpp"

using namespace std;

SessionManager::SessionManager(){

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
      sessions[nc].set_mg_connection(nc);
      break;
    }

    // New data on existing connection
    case MG_EV_RECV: {
      int recevied_bytes = *((int*)event_data);

      // Allocate a string to fit the recevied data
      string message;
      message.resize(recevied_bytes);

      // Copy recived contents to local buffer
      memcpy(&message[0], &nc->recv_mbuf, recevied_bytes);

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
