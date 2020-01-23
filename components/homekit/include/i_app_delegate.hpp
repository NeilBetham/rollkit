#ifndef I_APP_DELEGATE_HPP
#define I_APP_DELEGATE_HPP

#include <string>

#include "mongoose.h"


class IAppDelegate {
public:
  IAppDelegate();
  virtual ~IAppDelegate() {};
  virtual void handle_request(std::string& data) = 0;
};

#endif // I_APP_DELEGATE_HPP
