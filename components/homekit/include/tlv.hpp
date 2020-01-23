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

  uint8_t get_type() const { return tlv_type; };
  uint64_t length() const { return tlv_size; };
  std::string get_value() const { return tlv_value; };
  std::string serialize() const;

  static std::vector<TLV> decode(const std::string& data);
  static std::string encode(const std::vector<TLV>& tlvs);

private:
  uint8_t tlv_type;
  uint64_t tlv_size;
  std::string tlv_value;

  static TLV combine_tlvs(const std::vector<TLV>& tlvs);
};



#endif // TLV_HPP
