#ifndef SRP_MATH_HPP
#define SRP_MATH_HPP

#include <string>
#include <vector>

#include "bignum.hpp"
#include "i_hash_function.hpp"


namespace SRP {


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


} // namespace SRP

#endif // SRP_MATH_HPP
