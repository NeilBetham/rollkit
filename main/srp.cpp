#include "srp.hpp"

#include <sodium.h>
#include <ios>
#include <algorithm>
#include <sstream>

BigInt SRP::hash(int byte_count, const std::vector<BigInt>& h){

}


BigInt SRP::random(int byte_count){
  return SRP::External::random(byte_count);
}

// Hex conversion into and out of hash
std::string SRP::to_hex(const BigInt& from_int){
  BigInt temp = from_int;
  std::stringstream hex_str;

  while(temp > 0){
    BigInt remainder = temp % 16;
    temp = temp / 16;
    hex_str << hex << remainder.to_int();
  }

  std::string output = hex_str.str();
  std::reverse(output.begin(). output.end());

  return output;
}

BigInt SRP::to_bigint(const std::string& from_string){
  BitInt ret;
  int index = from_string.size();

  for(auto &elem : from_string){
    index --;
    int hex_val = 0;

    if (elem >= '0' && elem <= '9')
        hex_val = elem - '0';
    if (elem >= 'A' && elem <= 'F')
        hex_val = elem - 'A' + 10;
    if (elem >= 'a' && elem <= 'f')
        hex_val = elem - 'a' + 10;

    ret += (hex_val * std::pow(16, index));
  }

  return ret;
}


BigInt SRP::External::sha512(const BigInt& value){
  std::string value_s = value.to_string();
  std::string temp;
  temp.reserve(value_s.size());

  crypto_hash_sha512((unsigned char*)temp.data(), (unsigned char*)value_s.data(), value_s.size());

  return BigInt(temp);
}

BigInt SRP::External::random(int byte_count){
  std::string temp;
  temp.reserve(byte_count);

  randombytes_buf((void*) temp.data(), byte_count);

  return BigInt(temp);
}
