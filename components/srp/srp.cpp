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

BigInt bxor(const BigInt& a, const BigInt& b) {
  string a_s = a.to_string();
  string b_s = b.to_string();
  string out;
  out.reserve(max(a_s.size(), b_s.size()));

  if(a_s.size() > b_s.size()) {
    add_leading_zeroes(b_s, a_s.size() - b_s.size());
  } else {
    add_leading_zeroes(a_s, a_s.size() - b_s.size());
  }

  for(int i = 0; i < out.capacity(); i++) {
    out = a_s[i] ^ b_s[i];
  }

  return BigInt(out);
}


BigInt hash(const BigInt& prime_n, const vector<BigInt>& h) {
  uint64_t safe_prime_size = to_hex(prime_n).size();
  string to_hash;

  for(auto &val_to_pad: h) {
    string val_hex = to_hex(val_to_pad);
    for(int i = val_hex.size(); i < safe_prime_size; i++) {
      val_hex.insert(val_hex.begin(), '0');
    }
    to_hash += val_hex;
  }

  return to_bigint(External::sha512(to_hash)) % prime_n;
}


BigInt random(int byte_count) {
  return External::random(byte_count);
}

// Hex conversion into and out of hash
string to_hex(const BigInt& from_int) {
  BigInt temp = from_int;
  stringstream hex_str;

  while(temp > 0) {
    BigInt remainder = temp % 16;
    temp = temp / 16;
    hex_str << hex << remainder.to_int();
  }

  string output = hex_str.str();
  reverse(output.begin(), output.end());

  return output;
}

BigInt to_bigint(const string& hex_string) {
  BigInt ret;
  long index = hex_string.size();

  ESP_LOGI("SRP to_bigint LOOP", "START");
  for(auto &elem : hex_string) {
    index --;
    BigInt hex_val = 0;

    if (elem >= '0' && elem <= '9')
        hex_val = elem - '0';
    if (elem >= 'A' && elem <= 'F')
        hex_val = elem - 'A' + 10;
    if (elem >= 'a' && elem <= 'f')
        hex_val = elem - 'a' + 10;

    ret += (hex_val * pow(BigInt(16), index));
  }
  ESP_LOGI("SRP to_bigint LOOP", "END");

  return ret;
}


// a^n (mod m)
BigInt mod_pow(const BigInt& b, const BigInt& e, const BigInt& m) {
  BigInt result(1);
  BigInt base = b % m;
  BigInt exponent(e);
  ESP_LOGI("SRP ModPow LOOP", "START");
  fflush(stdout);
  vTaskDelay(1);
  while(exponent > 0) {
    if(exponent % 2 == 1) {
      result = (result * base) % m;
    }
    exponent /= 2;
    base = (base * base) % m;
    printf(".");
    fflush(stdout);
    vTaskDelay(1);
  }
  ESP_LOGI("SRP ModPow LOOP", "END");
  fflush(stdout);
  vTaskDelay(1);
  return result;
}

// k = H(n, g)
BigInt get_k(const BigInt& n, const BigInt& g) {
  return hash(n, {n , g});
}

// x = H(salt | H(username | ':' | password))
BigInt get_x(const std::string& username, const std::string& password, const BigInt& salt) {
  string salt_s = salt.to_string();
  if ((salt_s.size() % 2) != 0) {
    salt_s.insert(salt_s.begin(), '0');
  }
  string sup_concat = salt_s + username + ":" + password;
  return BigInt(External::sha512(sup_concat));
}

// u = H(A, B)
BigInt get_u(const BigInt& n, const BigInt& A, const BigInt& B) {
  return hash(n, {A, B});
}

// v = g^x (mod n)
BigInt get_v(const BigInt& n, const BigInt& g, const BigInt& x) {
  return mod_pow(g, x, n);
}

// A = g^a (mod n)
BigInt get_A(const BigInt& n, const BigInt& g, const BigInt& a) {
  return mod_pow(g, a, n);
}

// B = g^b + k v (mod N)
BigInt get_B(const BigInt& n, const BigInt& g, const BigInt& b, const BigInt& k, const BigInt& v) {
  return (mod_pow(g, b, n) + (k * v)) % n;
}

// Server S = (A * v^u) ^ b % n
BigInt get_S_host(const BigInt& n, const BigInt& A, const BigInt& v, const BigInt& u, const BigInt& b) {
  return mod_pow(A * mod_pow(v, u, n), b, n);
}

// K = H(S)
BigInt get_K(const BigInt& S) {
  return BigInt(External::sha512(S.to_string()));
}

// Client M = H(H(N) xor H(g), H(I), s, A, B, K)
BigInt get_M_client(const string& username, const BigInt& n, const BigInt& g, const BigInt& s, const BigInt& A, const BigInt& B, const BigInt& K) {
  BigInt hn = BigInt(External::sha512(n.to_string()));
  BigInt hg = BigInt(External::sha512(g.to_string()));
  BigInt hxor = bxor(hn, hg);
  BigInt hu = BigInt(External::sha512(username));
  return hash(n, {hxor, hu, s, A, B, K});
}

// Host M = H(A, M, K)
BigInt get_M_host(const BigInt& n, const BigInt& A, const BigInt& M, const BigInt& K) {
  return hash(n , {A, M, K});
}


// class User
User User::fromPassword(const BigInt& n, const BigInt& g, const std::string &username, const std::string &password) {
  BigInt rand_salt = random(32);
  BigInt x = get_x(username, password, rand_salt);
  BigInt v = get_v(n, g, x);

  return User(username, v, rand_salt);
}


// class Verifier
Challenge Verifier::get_challenge(const BigInt& _A) {
  if((A % N) == 0) {
    A = _A;
  }

  k = get_k(N, g);
  b = random(32);
  B = get_B(N, g, b, k, user.get_verifier());
  return Challenge(B, user.get_salt());
}

bool Verifier::verify(const BigInt& client_M) {
  bool verified = false;
  BigInt u = hash(N , {A, B});
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

BigInt random(int byte_count) {
  string temp;
  temp.reserve(byte_count);

  randombytes_buf((void*) temp.data(), byte_count);

  return BigInt(temp);
}


} // namespace External
} // namespace SRP
