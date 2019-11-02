#ifndef SRP_HPP
#define SRP_HPP

#include <vector>

#include "bignum.hpp"
#include "tommath.h"

namespace SRP {

// Interface to external libs
BigNum hash(const BigNum& safe_prime_n, const std::vector<BigNum>& h);
BigNum random(int byte_count);

// Hex conversion into and out of hash
std::string to_hex(const BigNum& from_int);
BigNum to_bigint(const std::string& hex_string);

// a^n (mod m)
BigNum mod_pow(const BigNum& a, const BigNum& n, const BigNum& m);

// k = H(n, g)
BigNum get_k(const BigNum& n, const BigNum& g);

// x = H(salt || H(username || ":" || password))
BigNum get_x(const std::string& username, const std::string& password, const BigNum& salt);

// u = H(A, B)
BigNum get_u(const BigNum& n, const BigNum& A, const BigNum& B);

// v = g^x (mod n)
BigNum get_v(const BigNum& n, const BigNum& g, const BigNum& x);

// A = g^a (mod N)
BigNum get_A(const BigNum& n, const BigNum& g, const BigNum& a);

// B = g^b + k v (mod N)
BigNum get_B(const BigNum& n, const BigNum& g, const BigNum& b, const BigNum& k, const BigNum& v);

// Server S = (A * v^u) ^ b % n
BigNum get_S_host(const BigNum& n, const BigNum& A, const BigNum& v, const BigNum& u, const BigNum& b);

// K = H(S)
BigNum get_K(const BigNum& S);

// Client M = H(H(N) xor H(g), H(I), s, A, B, K)
BigNum get_M_client(const std::string& username, const BigNum& n, const BigNum& g, const BigNum& s, const BigNum& A, const BigNum& B, const BigNum& K);

// Host M = H(A, M, K)
BigNum get_M_host(const BigNum& n, const BigNum& A, const BigNum& M, const BigNum& K);


// User paramter class
class User {
public:
  User(const std::string _username, const BigNum& _verifier, const BigNum& _salt) : username(_username), verifier(_verifier), salt(_salt) {};

  static User fromPassword(const BigNum& n, const BigNum& g, const std::string& username, const std::string& password);

  std::string get_username() { return username; };
  BigNum get_verifier() { return verifier; };
  BigNum get_salt() { return salt; };

private:
  std::string username;
  BigNum verifier;
  BigNum salt;
};


// Holds challenge paramters to send to client
class Challenge {
public:
  Challenge(const BigNum& _Be, const BigNum& _salt) : Be(_Be), salt(_salt) {};

  BigNum get_B() { return Be; };
  BigNum get_salt() { return salt; };

private:
  BigNum Be;
  BigNum salt;
};


// Handles verification of SRP parameters
class Verifier {
  public:
    Verifier(const BigNum& prime, int _g, const User& _user);

    Challenge get_challenge(const BigNum& _A);
    bool verify(const BigNum& M);
    BigNum get_client_proof() { return H_AMK; };

  private:
    User user;

    BigNum N;
    int g;
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
  std::string sha512(const std::string& value);
  BigNum random(int byte_count);
} // External


} // namespace SRP


#endif // SRP_HPP
