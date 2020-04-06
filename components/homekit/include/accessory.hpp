#ifndef ACCESSORY_HPP
#define ACCESSORY_HPP

#include <cstdint>
#include <list>

#include "instance_id_manager.hpp"
#include "json.hpp"
#include "service.hpp"

class Accessory {
public:
  Accessory() : _instance_id(get_id_manager().get_acc_id()) {}

  void register_service(const Service& service) {
    _services.push_back(service);
  };

  nlohmann::json serialize() const;

private:
  uint64_t _instance_id;
  std::list<Service> _services;
};


#endif // ACCESSORY_HPP
