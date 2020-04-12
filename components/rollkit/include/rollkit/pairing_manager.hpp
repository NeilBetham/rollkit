#ifndef PAIRING_MANAGER_HPP
#define PAIRING_MANAGER_HPP

#include <list>
#include <string>

#include "rollkit/nvs_manager.hpp"
#include "rollkit/optional.hpp"

namespace rollkit {


class Pairing {
public:
  Pairing() {};
  Pairing(const std::string& pairing_id, const std::string& public_key) :
    _pairing_id(pairing_id), _public_key(public_key), _is_admin(true) {};

  std::string get_pairing_id() const { return _pairing_id; };
  std::string get_public_key() const { return _public_key; };
  bool is_admin() const { return _is_admin; };

private:
  std::string _pairing_id;
  std::string _public_key;
  bool _is_admin;
};


class PairingManager {
public:
  bool add_pairing(const Pairing& pairing);
  bool remove_pairing(const std::string& pairing_id);
  optional<Pairing> find_pairing(const std::string& pairing_id);
  std::list<Pairing> get_all_pairings();
  uint32_t get_pairing_count();

private:
  std::string get_next_pairing_key();
  uint32_t get_pairing_index();
  bool inc_pairing_index();
  optional<std::string> find_pairing_key(const std::string& pairing_id);


  NVSManager _nvsmgr;
};


} // namespace rollkit
#endif // PAIRING_MANAGER_HPP
