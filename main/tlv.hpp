#ifndef TLV_HPP
#define TLV_HPP

#include <cstdint>
#include <vector>
#include <forward_list>

class TLV {
public:
  TLV() : type(0) {};
  TLV(uint8_t _type, std::forward_list<uint8_t> _value): type(_type), value(_value) {};
  ~TLV() {};

  uint8_t type() { return type; };
  uint8_t length() { return value.size(); };
  std::forward_list<uint8_t> value() { return value };

  std::forward_list<uint8_t> serialize();

  static std::vector<TLV> decode(std::forward_list<uint8_t> data);
  static std::forward_list<uint8_t> encode(std::vector<TLV> tlvs);

private:
  uint8_t type;
  std::forward_list<uint8_t> value;
};



#endif // TLV_HPP
