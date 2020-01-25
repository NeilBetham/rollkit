#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>

#include "mongoose.h"
#include "i_session.hpp"

class Request {
public:
  Request(struct http_message msg, ISession& session) :
    _body(msg.body.p, msg.body.len),
    _method(msg.method.p, msg.method.len),
    _uri(msg.uri.p, msg.uri.len),
    _proto(msg.proto.p, msg.proto.len),
    _query_string(msg.query_string.p, msg.query_string.len),
    _session(session) {};

  std::string get_body() { return _body; };
  std::string get_method() { return _method; };
  std::string get_uri() { return _uri; };
  std::string get_proto() { return _proto; };
  std::string get_query_string() { return _query_string; };
  ISession& get_session() { return _session; };

private:
  std::string _body;
  std::string _method;
  std::string _uri;
  std::string _proto;
  std::string _query_string;
  ISession& _session;
};

#endif // REQUEST_HPP
