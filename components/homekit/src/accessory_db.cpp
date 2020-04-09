#include "accessory_db.hpp"

using namespace std;

void AccessoryDB::register_accessory(const Accessory& acc) {
  _accessories.push_back(acc);
  const auto& acc_ref = _accessories.back();
  _acc_map[acc_ref.get_id()] = &acc_ref;
  for(auto& srvc : acc_ref.get_services()) {
    for(auto& characteristic : srvc.get_characteristics()) {
      _char_map[characteristic.get_id()] = &characteristic;
    }
  }
}

optional<Accessory> AccessoryDB::find_acc(uint64_t acc_id) {
  if(_acc_map.find(acc_id) == _acc_map.end()) { return {}; }
  return {*_acc_map[acc_id]};
}

optional<Characteristic> AccessoryDB::find_char(uint64_t acc_id, uint64_t char_id) {
  if(_char_map.find(char_id) == _char_map.end()) { return {}; }
  return {*_char_map[char_id]};
}
