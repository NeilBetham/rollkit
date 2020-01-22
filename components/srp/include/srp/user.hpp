#ifndef SRP_USER_HPP
#define SRP_USER_HPP

#include <string>

#include "srp/bignum.hpp"
#include "srp/math.hpp"

namespace SRP {


// User paramter class
class User {
public:
  User(Math& math, const std::string _username, const BigNum& _verifier, const BigNum& _salt) :
    _math(math), username(_username), verifier(_verifier), salt(_salt) {};

  static User fromPassword(Math& math, const std::string& username, const std::string& password);

  std::string get_username() { return username; };
  BigNum get_verifier() { return verifier; };
  BigNum get_salt() { return salt; };

private:
  Math& _math;
  std::string username;
  BigNum verifier;
  BigNum salt;
};


} // namespace SRP

#endif // SRP_USER_HPP
