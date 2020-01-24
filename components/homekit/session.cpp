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
  ESP_LOGD("session", "Handling message of %u bytes...", data.size());
  // Parse the http data
  struct http_message message;
  int result = mg_parse_http(data.c_str(), data.size(), &message, 1);
  if(result < 1){
    // There was an error parsing the message so return
    // TODO: Add error handling

    string data_hex;
    data_hex.reserve(data.size() * 2);
    for(auto& byte : data) {
      uint8_t nyble_h = (byte & 0xF0) >> 4;
      uint8_t nyble_l = byte & 0x0F;

      if(nyble_h < 10) {
        data_hex.push_back(nyble_h + 0x30);
      } else {
        data_hex.push_back((nyble_h - 10) + 0x41);
      }

      if(nyble_l < 10) {
        data_hex.push_back(nyble_l + 0x30);
      } else {
        data_hex.push_back((nyble_l - 10) + 0x41);
      }
    }

    ESP_LOGD("session", "Error parsing http message: %i", result);
    ESP_LOGD("session", "Errored Request: %u - `%s`", data_hex.size(), data_hex.c_str());
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
