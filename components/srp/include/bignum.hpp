#ifndef BIGNUM_HPP
#define BIGNUM_HPP

#include <string>
#include <optional>
#include <cassert>

#include "tommath.h"

class BigNum {
public:
  BigNum() {
    mp_err error = mp_init(&_value);
    assert(error == MP_OKAY);
    mp_zero(&_value);
  };

  BigNum(const BigNum& other) {
    mp_err error = mp_init_copy(&_value, &other._value);
    assert(error == MP_OKAY);
  };

  BigNum(BigNum&& other) {
    mp_exch(&_value, &other._value);
  }

  BigNum(const std::string& b16_string) {
    mp_err error = mp_init_set(&_value, 0);
    assert(error == MP_OKAY);
    import_b16(b16_string);
  }

  BigNum(int val) {
    mp_err error = mp_init_set(&_value, val);
    assert(error == MP_OKAY);
  }

  ~BigNum() {
    mp_clear(&_value);
  };

  static BigNum from_raw(const std::string& raw_data) {
    BigNum ret;
    mp_err error = mp_import(&ret._value, raw_data.size(), 1, 1, 1, 0, raw_data.data());
    assert(error == MP_OKAY);
    return ret;
  };

  std::string export_raw() const {
    std::string tmp(mp_unsigned_bin_size(&_value), 0);
    mp_err error = mp_to_unsigned_bin(&_value, (unsigned char*)tmp.data());
    assert(error == MP_OKAY);
    return tmp;
  }

  BigNum& operator=(const BigNum& rhs) {
    mp_err error = mp_init_copy(&_value, &rhs._value);
    assert(error == MP_OKAY);
    return *this;
  }

  BigNum& operator=(BigNum&& rhs) {
    mp_exch(&_value, &rhs._value);
    return *this;
  }

  bool operator==(const BigNum& rhs) const {
    mp_ord res = mp_cmp(&_value, &rhs._value);
    if(res == MP_EQ) {
      return true;
    }
    return false;
  }

  bool operator==(int rhs) const {
    mp_ord res = mp_cmp_d(&_value, rhs);
    if(res == MP_EQ) {
      return true;
    }
    return false;
  }

  BigNum operator+(const BigNum& rhs) const {
    BigNum ret;
    return check_ret(mp_add(&_value, &rhs._value, &ret._value), ret);
  };

  BigNum operator*(const BigNum& rhs) const {
    BigNum ret;
    return check_ret(mp_mul(&_value, &rhs._value, &ret._value), ret);
  };

  BigNum operator%(const BigNum& rhs) const {
    BigNum ret;
    return check_ret(mp_mod(&_value, &rhs._value, &ret._value), ret);
  };

  BigNum operator^(const BigNum& rhs) const {
    BigNum ret;
    return check_ret(mp_xor(&_value, &rhs._value, &ret._value), ret);
  }

  BigNum mod_pow(const BigNum& exponent, const BigNum& modulus) const {
    BigNum ret;
    return check_ret(mp_exptmod(&_value, &exponent._value, &modulus._value, &ret._value), ret);
  };

  bool import_b64(const std::string& data) {
    if(mp_read_radix(&_value, data.data(), 64) == MP_OKAY) {
      return true;
    }
    return false;
  };

  std::string export_b64() const {
    int buf_size = 0;
    mp_err error = mp_radix_size(&_value, 64, &buf_size);
    if(error != MP_OKAY) { return ""; }

    std::string buffer(buf_size, 0);
    error = mp_toradix(&_value, &buffer[0], 64);
    if(error != MP_OKAY) { return ""; }
    return buffer;
  };

  bool import_b16(const std::string& data) {
    if(mp_read_radix(&_value, data.data(), 16) == MP_OKAY) {
      return true;
    }
    return false;
  };

  std::string export_b16() const {
    int buf_size = 0;
    mp_err error = mp_radix_size(&_value, 16, &buf_size);
    if(error != MP_OKAY) { return ""; }

    std::string buffer(buf_size, 0);
    error = mp_toradix(&_value, &buffer[0], 16);
    if(error != MP_OKAY) { return ""; }
    return buffer;
  };


private:
  BigNum check_ret(mp_err err, const BigNum& ret) const {
    if(err != MP_OKAY) {
      return BigNum();
    }
    return ret;
  };

  mp_int _value;
};


#endif // BIGNUM_HPP
