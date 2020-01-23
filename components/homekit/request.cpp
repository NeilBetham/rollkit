#include <esp_log.h>

#include "request.hpp"


Request::Request(struct http_message msg, mg_connection* connection) :
  body(msg.body.p, msg.body.len),
  method(msg.method.p, msg.method.len),
  uri(msg.uri.p, msg.uri.len),
  proto(msg.proto.p, msg.proto.len),
  query_string(msg.query_string.p, msg.query_string.len) {
    ESP_LOGD("REQUEST-NEW", "URI: %s", uri.c_str());
  }
