#ifndef SERVICE_HPP
#define SERVICE_HPP

#include <cstdint>
#include <list>
#include <string>

#include "characteristic.hpp"
#include "instance_id_manager.hpp"
#include "utils.hpp"

class Service {
public:
  Service() {};
  Service(uint32_t hap_type, bool hidden, bool primary) :
    _hap_type(hap_uuid_prefix(hap_type)), _instance_id(get_id_manager().get_serv_char_id()),
    _hidden(hidden), _primary(primary) {};

  uint64_t get_id() const { return _instance_id; };
  const std::list<Characteristic>& get_characteristics() const { return _characteristics; };

  void register_characteristic(const Characteristic& characteristic) {
    _characteristics.push_back(characteristic);
  };
  nlohmann::json serialize() const;

private:
  std::list<Characteristic> _characteristics;

  std::string _hap_type;
  uint64_t _instance_id;
  bool _hidden;
  bool _primary;
  std::vector<uint64_t> _linked_serivces;
};


#endif // SERVICE_HPP
