#ifndef ACCESSORY_DB_HPP
#define ACCESSORY_DB_HPP

#include <unordered_map>

#include "accessory.hpp"
#include "optional.hpp"

class AccessoryDB {
public:
  void register_accessory(const Accessory& acc);
  optional<Accessory> find_acc(uint64_t acc_id);
  optional<Characteristic> find_char(uint64_t acc_id, uint64_t char_id);
  const std::list<Accessory>& get_accessories() { return _accessories; };

private:
  std::list<Accessory> _accessories;
  std::unordered_map<uint64_t, const Accessory*> _acc_map;
  std::unordered_map<uint64_t, const Characteristic*> _char_map;
};


#endif // ACCESSORY_DB_HPP
