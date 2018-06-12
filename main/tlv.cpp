#include "tlv.hpp"

using namespace std;

forward_list<uint8_t> TLV::serialize() {
  forward_list<uint8_t> ret;

  int byte_count = 0;
  int header_count = 0;
  for(auto &byte : tlv_value){
    if(byte_count == 0){
      // Time to render a header
      ret.insert_after(ret.end(), tlv_type);
      if((length() - (header_count * 255)) > 255){
        ret.insert_after(ret.end(), 255);
      } else {
        ret.insert_after(ret.end(), length() - (header_count * 255));
      }
      header_count++;
    }

    ret.insert_after(ret.end(), byte);
    byte_count++;

    if(byte_count == 255){
      byte_count = 0;
    }
  }

  return ret;
}

vector<TLV> TLV::decode(forward_list<uint8_t> data) {
  vector<TLV> decoded_tlvs;

  uint8_t prev_tlv_type = -1;
  int16_t current_tlv_type = -1;
  int16_t current_tlv_size = -1;
  uint8_t value_bytes_consumed = 0;
  uint64_t total_tlv_size = 0;
  forward_list<uint8_t> current_tlv_value;
  bool possible_fragmentation = false;

  // Decode all the TLVs in the data stream
  for(auto &byte : data){
    if(current_tlv_type == -1) {
      current_tlv_type = byte;

      // Check if there may have been fragmentation
      // If not push the old TLV fragment into the vector
      if(possible_fragmentation && prev_tlv_type != current_tlv_type){
        decoded_tlvs.push_back(TLV(prev_tlv_type, total_tlv_size, current_tlv_value));
        current_tlv_value.clear();
        possible_fragmentation = false;
      }
    } else if(current_tlv_size == -1) {
      current_tlv_size = byte;

      // If the size is 255 there may be fragmentation to the next TLV
      if(current_tlv_size == 255){
        possible_fragmentation = true;
      } else {
        possible_fragmentation = false;
      }
    } else if(current_tlv_size > value_bytes_consumed) {
      // Consume bytes till we have reached the size of the value
      current_tlv_value.insert_after(current_tlv_value.end(), byte);
      value_bytes_consumed++;
      total_tlv_size++;
    } else {
      // Handle creation of TLV instances
      if(!possible_fragmentation){
        decoded_tlvs.push_back(TLV(current_tlv_type, total_tlv_size, current_tlv_value));
        current_tlv_value.clear();
      }

      prev_tlv_type = current_tlv_type;

      current_tlv_type = -1;
      current_tlv_size = -1;
      value_bytes_consumed = 0;
    }
  }

  return decoded_tlvs;
}

forward_list<uint8_t> TLV::encode(vector<TLV> tlvs) {
  forward_list<uint8_t> ret;
  for(auto &tlv : tlvs){
    auto serial_tlv = tlv.serialize();
    ret.insert_after(ret.end(), serial_tlv.begin(), serial_tlv.end());
  }

  return ret;
}
