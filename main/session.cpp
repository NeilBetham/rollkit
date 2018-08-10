#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>

#include "session.hpp"
#include "request.hpp"

using namespace std;

Session::Session(){
  delegate = NULL;
  is_pair_verified = false;;
  connection = NULL;
}

Session::~Session(){

}

void Session::handle_request(string& data){
  if(is_pair_verified == true){
    // Handle decryption
  } else {
    // Handle regular message
    handle_message(data);
  }
}

void Session::handle_message(string& data){
  // Parse the http data
  struct http_message message;
  int result = mg_parse_http(data.c_str(), data.size(), &message, 1);
  if(result < 1){
    // There was an error parsing the message so return
    // TODO: Add error handling
    ESP_LOGD("session", "Error parsing http message: %i", result);
    ESP_LOGD("session", "Errored Request: `%s`", data.c_str());
    return;
  }

  Request new_request(message, connection);
  ESP_LOGD("session", "Got Req With URI: %s", new_request.get_uri().c_str());
  if(delegate != NULL){
    delegate->request_recv(new_request, this);
  }
}

void Session::close(){
}

bool Session::is_closed(){
  return true;
}

void Session::head(int code){
}

void Session::send(int code, string& data){
}
