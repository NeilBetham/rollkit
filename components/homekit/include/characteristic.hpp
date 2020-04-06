#ifndef CHARACTERISTIC_HPP
#define CHARACTERISTIC_HPP

#include <cstdint>
#include <functional>
#include <list>
#include <string>

#include "json.hpp"
#include "instance_id_manager.hpp"
#include "utils.hpp"


class Characteristic {
public:
  Characteristic() {};
  Characteristic(
      uint32_t hap_type,
      std::function<void(std::string)> write_cb,
      std::function<std::string()> read_cb,
      std::string format,
      std::list<std::string> perms = {"pr","pw"}) :
      _hap_type(hap_uuid_prefix(hap_type)),
      _instance_id(get_id_manager().get_serv_char_id()),
      _write_cb(write_cb), _read_cb(read_cb), _perms(perms), _format(format) {};

    void handle_update(const std::string& value) {  _write_cb(value); };
    std::string handle_read() const { return _read_cb(); }

    nlohmann::json serialize() const;

private:
  std::string _hap_type;
  uint32_t _instance_id;
  mutable std::function<void(std::string)> _write_cb;
  mutable std::function<std::string()> _read_cb;
  std::list<std::string> _perms;
  std::string _format;
};


#endif // CHARACTERISTIC_HPP
