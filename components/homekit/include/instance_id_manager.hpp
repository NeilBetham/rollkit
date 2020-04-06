#ifndef INSTANCE_ID_MANAGER_HPP
#define INSTANCE_ID_MANAGER_HPP

namespace {

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

} // namespace


InstanceIDManager& get_id_manager() {
  static InstanceIDManager id_mgr;
  return id_mgr;
}

#endif // INSTANCE_ID_MANAGER_HPP
