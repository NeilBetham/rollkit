#include "router.hpp"

using namespace std;

void Router::register_route(std::string uri, IRoute* route){
  routes.push_back(
    make_tuple(
      uri,
      route
    )
  );
}


void Router::route_request(Request& request){
  for(auto &route_tuple : routes){
    if(get<0>(route_tuple) == request.get_uri()){
      get<1>(route_tuple)->handle_request(request);
    }
  }
}
