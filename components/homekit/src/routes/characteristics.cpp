#include "routes/characteristics.hpp"

#include <list>

#include "utils.hpp"

using namespace std;

namespace routes {

void Characteristics::handle_request(Request& request, std::list<Accessory>& accessories) {
  auto query_info = parse_query_string(request.get_query_string());
  list<uint64_t> id_list;

  for(auto& kv : query_info) {
    string key;
    string value;
    tie(key, value) = kv;
    if(key == "id") {
      
    }
  }
};

} // namespace routes
