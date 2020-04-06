#include "utils.hpp"

#include <stdio.h>

#include "esp_log.h"

using namespace std;

namespace internal {

uint8_t hex_c_to_bin_c(char hex) {
  uint8_t bin = 0;

  if(hex >= '0' && hex <= '9') {
    bin = hex - 0x30;
  } else if(hex >= 'a' && hex <= 'f') {
    bin  = (hex - 'a') + 10;
  } else if(hex >= 'A' && hex <= 'F') {
    bin  = (hex - 'A') + 10;
  }

  return bin;
}

tuple<string, string> parse_query_kv(string kv_s) {
  size_t equal_pos = kv_s.find("=");
  if(equal_pos == npos) { return {{}, {}}; }
  string key = kv_s.substr(0, equal_pos);
  string value = kv_s.substr(equal_pos + 1, kv_s.size() - (equal_pos + 1));
  ESP_LOGD("utils", "Key: %s Value: %s", key.c_str(), value.c_str());
  return {key, value};
}

} // namespace

string to_hex(const string& bin) {
  string data_hex;
  data_hex.reserve(bin.size() * 2);
  for(auto& byte : bin) {
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

string to_bin(const string& hex) {
  string output;
  bool high_nibble = true;
  for(auto& ch : hex) {
    auto bin_c = internal::hex_c_to_bin_c(ch);
    if(high_nibble) {
      output.push_back(bin_c << 4);
    } else {
      output.back() |= bin_c;
    }
    high_nibble = !high_nibble;
  }

  return output;
}

string hap_uuid_prefix(uint32_t value) {
  // Serialize Type ID
  string type(8, 0); // 32 Bit Hex
  int printed = snprintf(&type[0], type.size(), "%X", value);
  type.resize(printed);

  return type;
}

list<tuple<string, string>> parse_query_string(string qs) {
  size_t start_pos = 0;
  size_t current_pos = qs.find("&");
  list<tuple<string, string>> ret;

  while(current_pos != npos) {
    ret.push_back(parse_query_kv(qs.substr(start_pos, current_pos - start_pos)));
    start_pos = current_pos + 1;
    current_pos = find("&", start_pos);
  }

  return ret;
}

list<tuple<uint64_t, uint64_t>> parse_id_list(string id_list) {
  list<uint64_t> ret;
  size_t start_pos = 0;
  size_t current_pos = qs.find(",");

  while(current_pos != npos) {
    ret.push_back(parse_query_kv(qs.substr(start_pos, current_pos - start_pos)));
    start_pos = current_pos + 1;
    current_pos = find("&", start_pos);
  }
}
