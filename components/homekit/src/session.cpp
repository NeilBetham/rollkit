#include "session.hpp"

#include <esp_log.h>

#include "request.hpp"
#include "utils.hpp"

using namespace std;

void Session::handle_request(string& data){
  if(_is_pair_verified == true){
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

    ESP_LOGD("session", "Error parsing http message: %i", result);
    ESP_LOGD("session", "Errored Request: %u - `%s`", data.size(), to_hex(data).c_str());
    return;
  }

  Request new_request(message, *this);
  ESP_LOGD("session", "Got Req With URI: %s", new_request.get_uri().c_str());
  if(_delegate){
    _delegate->request_recv(new_request);
  }
}

void Session::close(){
  _connection->flags |= MG_F_CLOSE_IMMEDIATELY;
}

bool Session::is_closed(){
  return true;
}

void Session::head(int code){
  mg_send_head(_connection, code, 0, "Content-Type: text/plain");
}

void Session::send(int code, const string& data, const string& content_type){
  string header_fmt = "HTTP/1.1 %d %s\r\nContent-Length: %d\r\nContent-Type: %s\r\n\r\n";
  int buf_size = snprintf(NULL, 0, header_fmt.c_str(), code, mg_status_message(code), data.size(), content_type.c_str());
  string header(buf_size, 0);
  (void)sprintf(&(header[0]), header_fmt.c_str(), code, mg_status_message(code), data.size(), content_type.c_str());

  mg_send(_connection, header.c_str(), header.size());
  mg_send(_connection, data.c_str(), data.size());
  ESP_LOGD("session", "Sending %u bytes", data.size());
}
