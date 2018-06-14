#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>

#include "mongoose.h"

class Request {
public:
  Request(){};
  Request(struct http_message msg, mg_connection* connection);

  std::string get_body(){ return body; };
  std::string get_method(){ return method; };
  std::string get_uri(){ return uri; };
  std::string get_proto(){ return proto; };
  std::string get_query_string(){ return query_string; };

private:
  std::string body;
  std::string method;
  std::string uri;
  std::string proto;
  std::string query_string;
};

#endif // REQUEST_HPP
