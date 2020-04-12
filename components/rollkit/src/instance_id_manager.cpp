#include "rollkit/instance_id_manager.hpp"

namespace rollkit {


InstanceIDManager& get_id_manager() {
  static InstanceIDManager id_mgr;
  return id_mgr;
}


} // namespace rollkit
