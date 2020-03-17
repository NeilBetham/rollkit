#include "srp/math.hpp"

#include <cstdint>

#include "srp/utils.hpp"

using namespace std;

namespace SRP {


BigNum Math::hash(const vector<BigNum>& h, bool pad) {
  uint64_t safe_prime_size = _prime.export_raw().size(); // TODO: Optimize
  string to_hash;

  for(auto& val : h) {
    string val_to_pad = val.export_raw();
    if(pad) {
      for(int i = val_to_pad.size(); i < safe_prime_size; i++) {
        val_to_pad.insert(val_to_pad.begin(), 0);
      }
    }
    to_hash += val_to_pad;
  }
  return BigNum::from_raw(_hash_fn.hash(to_hash)); // % prime_n;
}


// a^n (mod m)
BigNum Math::mod_pow(const BigNum& b, const BigNum& e){
  return SRP::mod_pow(b, e, _prime);
}


// k = H(n, g)
BigNum Math::get_k() {
  return hash({_prime, _generator});
}


// x = H(salt | H(username | ':' | password))
BigNum Math::get_x(const std::string& username, const std::string& password, const BigNum& salt) {
  string up_hash = _hash_fn.hash(username + ":" + password);
  string hash_val = _hash_fn.hash(salt.export_raw() + up_hash);
  return BigNum::from_raw(hash_val);
}


// u = H(A, B)
BigNum Math::get_u(const BigNum& A, const BigNum& B) {
  return hash({A, B});
}


// v = g^x (mod n)
BigNum Math::get_v(const BigNum& x) {
  return mod_pow(_generator, x);
}


// A = g^a (mod n)
BigNum Math::get_A(const BigNum& a) {
  return mod_pow(_generator, a);
}


// B = g^b + k v (mod N)
BigNum Math::get_B(const BigNum& b, const BigNum& k, const BigNum& v) {
  return (k * v + SRP::mod_pow(_generator, b, _prime)) % _prime;
}


// Server S = (A * v^u) ^ b % n
BigNum Math::get_S_host(const BigNum& A, const BigNum& v, const BigNum& u, const BigNum& b) {
  return mod_pow(A * mod_pow(v, u), b);
}


// K = H(S)
BigNum Math::get_K(const BigNum& S) {
  return hash({S});
}


// Client M = H(H(N) xor H(g), H(I), s, A, B, K)
BigNum Math::get_M_client(const string& username, const BigNum& s, const BigNum& A, const BigNum& B, const BigNum& K) {
  BigNum hn = BigNum::from_raw(_hash_fn.hash(_prime.export_raw()));
  BigNum hg = BigNum::from_raw(_hash_fn.hash(_generator.export_raw()));
  BigNum hu = BigNum::from_raw(_hash_fn.hash(username));
  return hash({hn ^ hg, hu, s, A, B, K}, false);
}


// Host M = H(A, M, K)
BigNum Math::get_M_host(const BigNum& A, const BigNum& M, const BigNum& K) {
  return hash({A, M, K}, false);
}


} // namespace SRP
