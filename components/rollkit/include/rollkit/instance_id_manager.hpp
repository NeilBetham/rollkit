#ifndef INSTANCE_ID_MANAGER_HPP
#define INSTANCE_ID_MANAGER_HPP

#include <cstdint>

namespace rollkit {


class InstanceIDManager {
public:
  InstanceIDManager() {}
  ~InstanceIDManager() {};

  uint64_t get_acc_id() {
    return _accessory_id_index++;
  }

  uint64_t get_serv_char_id() {
    return _serv_char_id_index++;
  }

private:
  uint64_t _accessory_id_index = 1;
  uint64_t _serv_char_id_index = 1;
};

InstanceIDManager& get_id_manager();


} // namespace rollkit
#endif // INSTANCE_ID_MANAGER_HPP
