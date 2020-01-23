#ifndef TLV_HPP
#define TLV_HPP

#include <cstdint>
#include <vector>
#include <string>

class TLV {
public:
  TLV() : tlv_type(0), tlv_size(0) {};
  TLV(uint8_t _type, uint64_t _size, std::string _value): tlv_type(_type), tlv_size(_size), tlv_value(_value) {};
  ~TLV() {};

  uint8_t get_type() { return tlv_type; };
  uint64_t length() { return tlv_size; };
  std::string get_value() { return tlv_value; };
  std::string serialize();

  static std::vector<TLV> decode(std::string data);
  static std::string encode(std::vector<TLV> tlvs);

private:
  uint8_t tlv_type;
  uint64_t tlv_size;
  std::string tlv_value;
};



#endif // TLV_HPP
