#ifndef TLV_HPP
#define TLV_HPP

#include <cstdint>
#include <vector>
#include <string>

#include "optional.hpp"

class TLVs;

class TLV {
public:
  TLV() : tlv_type(0), tlv_size(0) {};
  TLV(uint8_t type, std::string data) : tlv_type(type), tlv_size(data.size()), tlv_value(data) {};
  ~TLV() {};

  uint8_t get_type() const { return tlv_type; };
  uint64_t length() const { return tlv_size; };
  std::string get_value() const { return tlv_value; };
  std::string serialize() const;
  std::string get_value_hex() const;

  static TLVs decode(const std::string& data);
  static std::string encode(const std::vector<TLV>& tlvs);

private:
  TLV(uint8_t _type, uint64_t _size, std::string _value): tlv_type(_type), tlv_size(_size), tlv_value(_value) {};

  uint8_t tlv_type;
  uint64_t tlv_size;
  std::string tlv_value;

  static TLV combine_tlvs(const std::vector<TLV>& tlvs);
};


class TLVs {
public:
  TLVs(std::vector<TLV> tlvs) : _tlvs(tlvs) {};

  optional<TLV> find(uint8_t type);
  const std::vector<TLV>& get() { return _tlvs; };
  TLV& operator[](size_t index) { return _tlvs[index]; };
  size_t size() { return _tlvs.size(); };

private:
  std::vector<TLV> _tlvs;
};


#endif // TLV_HPP
