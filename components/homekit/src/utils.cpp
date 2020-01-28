#include "utils.hpp"

using namespace std;


string to_hex(const string& target) {
  string data_hex;
  data_hex.reserve(target.size() * 2);
  for(auto& byte : target) {
    uint8_t nyble_h = (byte & 0xF0) >> 4;
    uint8_t nyble_l = byte & 0x0F;

    if(nyble_h < 10) {
      data_hex.push_back(nyble_h + 0x30);
    } else {
      data_hex.push_back((nyble_h - 10) + 0x41);
    }

    if(nyble_l < 10) {
      data_hex.push_back(nyble_l + 0x30);
    } else {
      data_hex.push_back((nyble_l - 10) + 0x41);
    }
  }

  return data_hex;
}
