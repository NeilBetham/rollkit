#ifndef PAIRING_MANAGER_HPP
#define PAIRING_MANAGER_HPP

#include <string>

#include "nvs_manager.hpp"
#include "optional.hpp"

class Pairing {
public:
  Pairing() {};
  Pairing(const std::string& pairing_id, const std::string& public_key) :
    _pairing_id(pairing_id), _public_key(public_key) {};

  std::string get_pairing_id() const { return _pairing_id; };
  std::string get_public_key() const { return _public_key; };

private:
  std::string _pairing_id;
  std::string _public_key;
};


class PairingManager {
public:
  bool add_pairing(const Pairing& pairing);
  bool remove_pairing(const std::string& pairing_id);
  optional<Pairing> find_pairing(const std::string& pairing_id);

private:
  std::string get_pairing_key();
  optional<std::string> find_pairing_key(const std::string& pairing_id);

  NVSManager _nvsmgr;
};


#endif // PAIRING_MANAGER_HPP
