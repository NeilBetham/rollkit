#include "srp/user.hpp"

#include "srp/utils.hpp"

#include "esp_log.h"

namespace SRP {


User User::fromPassword(Math& math, const std::string &username, const std::string &password) {
  BigNum rand_salt = SRP::random(16);

  BigNum x = math.get_x(username, password, rand_salt);
  BigNum v = math.get_v(x);

  return User(math, username, v, rand_salt);
}


} // namespace SRP
