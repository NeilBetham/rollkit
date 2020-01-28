#include "tlv.hpp"

#include <utility>

#include "utils.hpp"

using namespace std;

string TLV::serialize() const {
  string ret;

  int byte_count = 0;
  int header_count = 0;
  for(auto &byte : tlv_value){
    if(byte_count == 0){
      // Time to render a header
      ret.push_back(tlv_type);
      if((length() - (header_count * 255)) > 255){
        ret.push_back(255);
      } else {
        ret.push_back(length() - (header_count * 255));
      }
      header_count++;
    }

    ret.push_back(byte);
    byte_count++;

    if(byte_count == 255){
      byte_count = 0;
    }
  }

  return ret;
}

string TLV::get_value_hex() const {
  return to_hex(tlv_value);
}

TLVs TLV::decode(const string& data) {
  vector<TLV> decoded_tlvs;
  vector<TLV> defraged_tlvs;

  for(auto it = data.begin(); it < data.end();) {
    int16_t current_tlv_type = *it++;
    if(it > data.end()) { return TLVs({}); } // Safety check to prevent reading reading out of bounds

    int16_t current_tlv_size = *it++;
    if(it > data.end()) { return TLVs({}); } // Safety check to prevent reading reading out of bounds

    string current_tlv_value;

    current_tlv_value.reserve(current_tlv_size);
    for(uint64_t count = 0; count < current_tlv_size; count++) {
      if(it > data.end()) { return TLVs({}); } // Safety check to prevent reading reading out of bounds
      current_tlv_value.push_back(*it++);
    }

    decoded_tlvs.push_back(TLV(current_tlv_type, current_tlv_size, current_tlv_value));
  }

  vector<TLV> fragments;
  for(auto& tlv : decoded_tlvs) {
    // Check to see if this TLV matches the fragments we already have
    if(fragments.size() > 0) {
      if(fragments.back().get_type() != tlv.get_type() || fragments.back().length() < 255) {
        // This fragment has a different type or the last fragment was the end of
        // the previous fragmented message and this fragment is a new message.
        defraged_tlvs.push_back(combine_tlvs(fragments));
        fragments.clear();
      }
    }

    // We always want to push this fragment onto the list
    fragments.push_back(tlv);
  }

  // Empty out any remaining fragments
  if(fragments.size() > 0) {
    defraged_tlvs.push_back(combine_tlvs(fragments));
  }

  return TLVs(defraged_tlvs);
}

string TLV::encode(const vector<TLV>& tlvs) {
  string ret;
  for(auto &tlv : tlvs){
    auto serial_tlv = tlv.serialize();
    ret += serial_tlv;
  }

  return ret;
}

TLV TLV::combine_tlvs(const vector<TLV>& tlvs) {
  int16_t type = tlvs.front().get_type();
  uint64_t size = 0;
  string value;

  for(auto& frag : tlvs) {
    size += frag.length();
    value += frag.get_value();
  }

  return TLV(type, size, move(value));
}


optional<TLV> TLVs::find(uint8_t type) {
  for(auto& tlv : _tlvs) {
    if(tlv.get_type() == type) {
      return tlv;
    }
  }

  return {};
}
