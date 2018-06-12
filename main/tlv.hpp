#ifndef TLV_HPP
#define TLV_HPP

#include <cstdint>
#include <vector>
#include <forward_list>

class TLV {
public:
  TLV() : tlv_type(0), tlv_size(0) {};
  TLV(uint8_t _type, uint64_t _size, std::forward_list<uint8_t> _value): tlv_type(_type), tlv_size(_size), tlv_value(_value) {};
  ~TLV() {};

  uint8_t get_type() { return tlv_type; };
  uint64_t length() { return tlv_size; };
  std::forward_list<uint8_t> get_value() { return tlv_value; };

  std::forward_list<uint8_t> serialize();

  static std::vector<TLV> decode(std::forward_list<uint8_t> data);
  static std::forward_list<uint8_t> encode(std::vector<TLV> tlvs);

private:
  uint8_t tlv_type;
  uint64_t tlv_size;
  std::forward_list<uint8_t> tlv_value;
};



#endif // TLV_HPP
