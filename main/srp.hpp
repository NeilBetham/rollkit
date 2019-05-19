#ifndef SRP_HPP
#define SRP_HPP

#include <vector>

#include "BigInt.hpp"

namespace SRP {

// Interface to external libs
BigInt hash(const BigInt& safe_prime_n, const std::vector<BigInt>& h);
BigInt random(int byte_count);

// Hex conversion into and out of hash
std::string to_hex(const BigInt& from_int);
BigInt to_bigint(const std::string& hex_string);

// a^n (mod m)
BigInt mod_pow(const BigInt& a, const BigInt& n, const BigInt& m);

// k = H(n, g)
BigInt get_k(const BigInt& n, const BigInt& g);

// u = H(A, B)
BigInt get_u(const BigInt& n, const BigInt& A, const BigInt& B);

// v = g^x (mod n)
BigInt get_v(const BigInt& n, const BigInt& g, const BigInt& x);

// B = g^b + k v (mod N)
BigInt get_B(const BigInt& n, const BigInt& g, const BigInt& b, const BigInt& k, const BigInt& v);

// Server S = (A * v^u) ^ b % n
BigInt get_S_host(const BigInt& n, const BigInt& A, const BigInt& v, const BigInt& u, const BigInt& b);

// K = H(S)
BigInt get_K(const BigInt& S);

// Client M = H(H(N) xor H(g), H(I), s, A, B, K)
BigInt get_M_client(const std::string& username, const BigInt& n, const BigInt& g, const BigInt& s, const BigInt& A, const BigInt& B, const BigInt& K);

// Host M = H(A, M, K)
BigInt get_M_host(const BigInt& n, const BigInt& A, const BigInt& M, const BigInt& K);


// User paramter class
class User {
public:
  User(const std::string _username, const BigInt& _verifier, const BigInt& _salt) : username(_username), verifier(_verifier), salt(_salt) {};

  static User fromPassword(const BigInt& n, const BigInt& g, const std::string& username, const std::string& password);

  std::string get_username() { return username; };
  BigInt get_verifier() { return verifier; };
  BigInt get_salt() { return salt; };

private:
  std::string username;
  BigInt verifier;
  BigInt salt;
};


// Holds challenge paramters to send to client
class Challenge {
public:
  Challenge(const BigInt& _Be, const BigInt& _salt) : Be(_Be), salt(_salt) {};

  BigInt get_B() { return Be; };
  BigInt get_salt() { return salt; };

private:
  BigInt Be;
  BigInt salt;
};


// Handles verification of SRP parameters
class Verifier {
  public:
    Verifier(const BigInt& prime, int _g, const User& _user);

    Challenge get_challenge(const BigInt& _A);
    bool verify(const BigInt& M);
    BigInt get_client_proof() { return H_AMK; };

  private:
    User user;

    BigInt N;
    int g;
    BigInt k;
    BigInt A;
    BigInt B;
    BigInt b;
    BigInt S;
    BigInt K;
    BigInt M;
    BigInt H_AMK;
};


namespace External {
  std::string sha512(const std::string& value);
  BigInt random(int byte_count);
} // External


} // namespace SRP


#endif // SRP_HPP
