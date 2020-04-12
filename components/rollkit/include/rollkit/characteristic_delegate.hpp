#ifndef CHARACTERISTIC_DELEGATE_HPP
#define CHARACTERISTIC_DELEGATE_HPP

#include <functional>

#include "json.hpp"

namespace rollkit {


class CharacteristicDelegate {
public:
  ~CharacteristicDelegate() {};

  nlohmann::json handle_read() = 0;
  void hanlde_write(nlohmann::json& value) = 0;
  void send_write(nlohmann::json& value)

  void set_update_callback(std::function<void(nlohmann::json)> update_cb) { _update_cb = update_cb; };

private:
  std::function<void(nlohmann::json)> _update_cb;
};


} // namespace rollkit
#endif // CHARACTERISTIC_DELEGATE_HPP
