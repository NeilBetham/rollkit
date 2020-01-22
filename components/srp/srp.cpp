#include "srp.hpp"

#include <sodium.h>
#include <ios>
#include <algorithm>
#include <sstream>
#include <optional>

#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

using namespace std;

namespace SRP {


BigNum mod_pow(const BigNum& b, const BigNum& e, const BigNum& M) {
  return b.mod_pow(e, M);
}


BigNum Math::hash(const vector<BigNum>& h) {
  uint64_t safe_prime_size = _prime.export_raw().size(); // TODO: Optimize
  string to_hash;

  for(auto& val : h) {
    string val_to_pad = val.export_raw();
    for(int i = val_to_pad.size(); i < safe_prime_size; i++) {
      val_to_pad.insert(val_to_pad.begin(), 0);
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
  BigNum hn = BigNum(_hash_fn.hash(_prime.export_b16()));
  BigNum hg = BigNum(_hash_fn.hash(_generator.export_b16()));
  BigNum hxor = hn ^ hg;
  BigNum hu = BigNum(_hash_fn.hash(username));
  return hash({hxor, hu, s, A, B, K});
}


// Host M = H(A, M, K)
BigNum Math::get_M_host(const BigNum& A, const BigNum& M, const BigNum& K) {
  return hash({A, M, K});
}


// class User
User User::fromPassword(Math& math, const std::string &username, const std::string &password) {
  BigNum rand_salt = random(32);
  BigNum x = math.get_x(username, password, rand_salt);
  BigNum v = math.get_v(x);

  return User(math, username, v, rand_salt);
}


// class Verifier
Challenge Verifier::get_challenge(const BigNum& _A) {
  if((A % _math.N()) == 0) {
    A = _A;
  }

  k = _math.get_k();
  b = random(32);
  B = _math.get_B(b, k, _user.get_verifier());
  return Challenge(_math, B, _user.get_salt());
}


bool Verifier::verify(const BigNum& client_M) {
  bool verified = false;
  BigNum u = _math.hash({A, B});
  S = _math.get_S_host(A, _user.get_verifier(), u, b);
  K = _math.get_K(S);
  M = _math.get_M_client(_user.get_username(), _user.get_salt(), A, B, K);

  if(M == client_M) {
    verified = true;
  }

  H_AMK = _math.get_M_host(A, M, K);
  return verified;
}


BigNum random(int byte_count) {
  return External::random(byte_count);
}


namespace External {


BigNum random(int byte_count) {
  string temp;
  temp.resize(byte_count);

  randombytes_buf((void*) temp.data(), byte_count);

  return BigNum(temp);
}


} // namespace External
} // namespace SRP
