#include "instance_id_manager.hpp"

InstanceIDManager& get_id_manager() {
 static InstanceIDManager id_mgr;
 return id_mgr;
}
