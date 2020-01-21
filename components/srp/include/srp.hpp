#ifndef SRP_HPP
#define SRP_HPP

#include <vector>

#include "bignum.hpp"
#include "i_hash_function.hpp"


namespace SRP {


BigNum mod_pow(const BigNum& b, const BigNum& e, const BigNum& M);


class Math {
public:
  Math(IHashFunction& hash_fn, const BigNum& prime, const BigNum& generator) :
    _hash_fn(hash_fn), _prime(prime), _generator(generator) {};

  // a^n (mod m)
  BigNum mod_pow(const BigNum& b, const BigNum& e);

  // k = H(n, g)
  BigNum get_k();

  // x = H(salt || H(username || ":" || password))
  BigNum get_x(const std::string& username, const std::string& password, const BigNum& salt);

  // u = H(A, B)
  BigNum get_u(const BigNum& A, const BigNum& B);

  // v = g^x (mod n)
  BigNum get_v(const BigNum& x);

  // A = g^a (mod N)
  BigNum get_A(const BigNum& a);

  // B = g^b + k v (mod N)
  BigNum get_B(const BigNum& b, const BigNum& k, const BigNum& v);

  // Server S = (A * v^u) ^ b % n
  BigNum get_S_host(const BigNum& A, const BigNum& v, const BigNum& u, const BigNum& b);

  // K = H(S)
  BigNum get_K(const BigNum& S);

  // Client M = H(H(N) xor H(g), H(I), s, A, B, K)
  BigNum get_M_client(const std::string& username, const BigNum& s, const BigNum& A, const BigNum& B, const BigNum& K);

  // Host M = H(A, M, K)
  BigNum get_M_host(const BigNum& A, const BigNum& M, const BigNum& K);

  BigNum hash(const std::vector<BigNum>& h);

  BigNum N() { return _prime; };
  BigNum g() { return _generator; };

private:
  IHashFunction& _hash_fn;
  BigNum _prime;
  BigNum _generator;
};


// Interface to external libs
BigNum random(int byte_count);

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


// Holds challenge paramters to send to client
class Challenge {
public:
  Challenge(Math& math, const BigNum& _Be, const BigNum& _salt) :
    _math(math), Be(_Be), salt(_salt) {};

  BigNum get_B() { return Be; };
  BigNum get_salt() { return salt; };

private:
  Math& _math;
  BigNum Be;
  BigNum salt;
};


// Handles verification of SRP parameters
class Verifier {
  public:
    Verifier(Math& math, const User& user) : _math(math), _user(user) {};

    Challenge get_challenge(const BigNum& _A);
    bool verify(const BigNum& M);
    BigNum get_client_proof() { return H_AMK; };

  private:
    Math& _math;
    User _user;

    BigNum k;
    BigNum A;
    BigNum B;
    BigNum b;
    BigNum S;
    BigNum K;
    BigNum M;
    BigNum H_AMK;
};


namespace External {


BigNum random(int byte_count);


} // External
} // namespace SRP


#endif // SRP_HPP
