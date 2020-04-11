#ifndef I_APP_HPP
#define I_APP_HPP

#include <string>

#include "accessory_db.hpp"
#include "event_manager.hpp"

class IApp {
public:
  IApp() {};
  virtual ~IApp() {};

  virtual AccessoryDB& get_acc_db() = 0;
  virtual EventManager& get_ev_mgr() = 0;
  virtual std::string get_setup_code() = 0;
};

#endif // I_APP_HPP
