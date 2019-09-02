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

BigNum bxor(const BigNum& a, const BigNum& b) {
  return a ^ b;
}


BigNum hash(const BigNum& prime_n, const vector<BigNum>& h) {
  uint64_t safe_prime_size = to_hex(prime_n).size();
  string to_hash;

  for(auto &val_to_pad: h) {
    string val_hex = to_hex(val_to_pad);
    for(int i = val_hex.size(); i < safe_prime_size; i++) {
      val_hex.insert(val_hex.begin(), '0');
    }
    to_hash += val_hex;
  }

  return BigNum(External::sha512(to_hash)) % prime_n;
}


BigNum random(int byte_count) {
  return External::random(byte_count);
}

// Hex conversion into and out of hash
string to_hex(const BigNum& from_int) {
  return from_int.export_b16();
}

// a^n (mod m)
BigNum mod_pow(const BigNum& b, const BigNum& e, const BigNum& m) {
  return b.mod_pow(e, m);
}

// k = H(n, g)
BigNum get_k(const BigNum& n, const BigNum& g) {
  return hash(n, {n , g});
}

// x = H(salt | H(username | ':' | password))
BigNum get_x(const std::string& username, const std::string& password, const BigNum& salt) {
  string salt_s = salt.export_b16();
  if ((salt_s.size() % 2) != 0) {
    salt_s.insert(salt_s.begin(), '0');
  }
  string sup_concat = salt_s + username + ":" + password;
  return BigNum(External::sha512(sup_concat));
}

// u = H(A, B)
BigNum get_u(const BigNum& n, const BigNum& A, const BigNum& B) {
  return hash(n, {A, B});
}

// v = g^x (mod n)
BigNum get_v(const BigNum& n, const BigNum& g, const BigNum& x) {
  return mod_pow(g, x, n);
}

// A = g^a (mod n)
BigNum get_A(const BigNum& n, const BigNum& g, const BigNum& a) {
  return mod_pow(g, a, n);
}

// B = g^b + k v (mod N)
BigNum get_B(const BigNum& n, const BigNum& g, const BigNum& b, const BigNum& k, const BigNum& v) {
  return (mod_pow(g, b, n) + (k * v)) % n;
}

// Server S = (A * v^u) ^ b % n
BigNum get_S_host(const BigNum& n, const BigNum& A, const BigNum& v, const BigNum& u, const BigNum& b) {
  return mod_pow(A * mod_pow(v, u, n), b, n);
}

// K = H(S)
BigNum get_K(const BigNum& S) {
  return BigNum(External::sha512(S.export_b16()));
}

// Client M = H(H(N) xor H(g), H(I), s, A, B, K)
BigNum get_M_client(const string& username, const BigNum& n, const BigNum& g, const BigNum& s, const BigNum& A, const BigNum& B, const BigNum& K) {
  BigNum hn = BigNum(External::sha512(n.export_b16()));
  BigNum hg = BigNum(External::sha512(g.export_b16()));
  BigNum hxor = bxor(hn, hg);
  BigNum hu = BigNum(External::sha512(username));
  return hash(n, {hxor, hu, s, A, B, K});
}

// Host M = H(A, M, K)
BigNum get_M_host(const BigNum& n, const BigNum& A, const BigNum& M, const BigNum& K) {
  return hash(n , {A, M, K});
}


// class User
User User::fromPassword(const BigNum& n, const BigNum& g, const std::string &username, const std::string &password) {
  BigNum rand_salt = random(32);
  BigNum x = get_x(username, password, rand_salt);
  BigNum v = get_v(n, g, x);

  return User(username, v, rand_salt);
}


// class Verifier
Challenge Verifier::get_challenge(const BigNum& _A) {
  if((A % N) == 0) {
    A = _A;
  }

  k = get_k(N, g);
  b = random(32);
  B = get_B(N, g, b, k, user.get_verifier());
  return Challenge(B, user.get_salt());
}

bool Verifier::verify(const BigNum& client_M) {
  bool verified = false;
  BigNum u = hash(N , {A, B});
  S = get_S_host(N, A, user.get_verifier(), u, b);
  K = get_K(S);
  M = get_M_client(user.get_username(), N, g, user.get_salt(), A, B, K);

  if(M == client_M) {
    verified = true;
  }

  H_AMK = get_M_host(N, A, M, K);
  return verified;
}

namespace External {


string sha512(const string& value) {
  string temp;
  temp.reserve(value.size());

  crypto_hash_sha512((unsigned char*)temp.data(), (unsigned char*)value.data(), value.size());

  return move(temp);
}

BigNum random(int byte_count) {
  string temp;
  temp.reserve(byte_count);

  randombytes_buf((void*) temp.data(), byte_count);

  return BigNum(temp);
}


} // namespace External
} // namespace SRP
