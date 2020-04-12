#ifndef I_APP_HPP
#define I_APP_HPP

#include <string>

#include "rollkit/accessory_db.hpp"
#include "rollkit/event_manager.hpp"

namespace rollkit {
namespace interfaces {


class IApp {
public:
  IApp() {};
  virtual ~IApp() {};

  virtual AccessoryDB& get_acc_db() = 0;
  virtual EventManager& get_ev_mgr() = 0;
  virtual std::string get_setup_code() = 0;
};


} // namespace interfaces
} // namespace rollkit
#endif // I_APP_HPP
