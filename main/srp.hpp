#ifndef SRP_HPP
#define SRP_HPP

#include <vector>

#include "BigInt.hpp"

namespace SRP {

// Interface to external libs
BigInt hash(int byte_count, const std::vector<BigInt>& h);
BigInt random(int byte_count);

// Hex conversion into and out of hash
std::string to_hex(const BigInt& from_int);
BigInt to_bigint(const std::string& from_string);

// a^n (mod m)
BigInt mod_pow(const BigInt& a, const BigInt& n, const BigInt& m);

// k = H(n, g)
BigInt get_k(const BigInt& n, const BigInt& g);

// x = H(salt | H(username | ':' | password))
BigInt get_x(const std::string& username, const std::string& password, const BigInt& salt);

// u = H(A, B)
BigInt get_u(const BigInt& n, const BigInt& A, const BigInt& B);

// v = g^x (mod n)
BigInt get_v(const BigInt& n, const BigInt& g, const BigInt& x);

// A = g^a (mod n)
BigInt get_A(const BigInt& n, const BigInt& g, const BigInt& a);

// B = g^b + k v (mod N)
BigInt get_B(const BigInt& n, const BigInt& g, const BigInt& b, const BigInt& k, const BigInt& v);

// Client S = (B - (k * g^x)) ^ (a + (u * x)) % n
BigInt get_S_client(const BigInt& n, const BigInt& B, const BigInt& k, const BigInt& g, const BigInt& x, const BigInt& a, const BigInt& u);

// Server S = (A * v^u) ^ b % n
BigInt get_S_host(const BigInt& n, const BigInt& A, const BigInt& v, const BigInt& u, const BigInt& b);

// K = H(S)
BigInt get_K(const BigInt& n, const BigInt& S);

// Client M = H(H(N) xor H(g), H(I), s, A, B, K)
BigInt get_M_client(const BigInt& n, const BigInt& g, const BigInt& I, const BigInt& s, const BigInt& A, const BigInt& B, const BigInt& K);

// Host M = H(A, M, K)
BigInt get_M_host(const BigInt& n, const BigInt& A, const BigInt& M, const BigInt& K);


// User paramter class
class User {
public:
  User(const std::string _username, const BigInt& _verifier, const BigInt& _salt) : username(_username), verifier(_verifier), salt(_salt) {};

  static User fromPassword(const std::string& username, const std::string& password);

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
  Challenge(const BigInt& _B, const BigInt& _salt) : B(_B), salt(_salt) {};

  BigInt get_B() { return B; };
  BigInt get_salt() { return salt; };

private:
  BigInt B;
  BigInt salt;
};


// Handles verification of SRP parameters
class Verifier {
  public:
    Verifier(const BigInt& prime, int _g, const User& _user);

    Challenge get_challenge();
    bool verify(const BigInt& M);

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
  BigInt sha512(const BigInt& value);
  BigInt random(int byte_count);
} // External


} // namespace SRP


#endif // SRP_HPP
