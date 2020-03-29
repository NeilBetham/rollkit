#include "session.hpp"

#include <esp_log.h>

#include "request.hpp"
#include "utils.hpp"
#include "hkdf.hpp"

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
  string local_data = data;

  // If session is verified try to decrypt
  if(_is_pair_verified) {
    ESP_LOGD("session", "Decrypting message");
    auto plain_message = _session_sec.decrypt(data);
    if(!plain_message) {
      ESP_LOGE("session", "Decryption failed; closing connection");
      close();
      return;
    }

    local_data = *plain_message;
  }

  // Parse the http data
  struct http_message message;
  int result = mg_parse_http(local_data.c_str(), local_data.size(), &message, 1);
  if(result < 1){
    // There was an error parsing the message so return
    // TODO: Add error handling

    ESP_LOGD("session", "Error parsing http message: %i", result);
    ESP_LOGD("session", "Errored Request: %u - `%s`", local_data.size(), to_hex(local_data).c_str());
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

void Session::setup_security(const string& shared_secret, bool is_admin) {
  _acc_to_cont_key = hkdf_sha512(
    shared_secret,
    "Control-Salt",
    "Control-Read-Encryption-Key",
    32
  );

  _cont_to_acc_key = hkdf_sha512(
    shared_secret,
    "Control-Salt",
    "Control-Write-Encryption-Key",
    32
  );

  _is_pair_verified = true;
  _is_admin = is_admin;
  _session_sec = SessionSecurity(_acc_to_cont_key, _cont_to_acc_key);
}
