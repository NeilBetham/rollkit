#include <string>

#include "unity.h"
#include "srp.hpp"

using namespace std;

TEST_CASE("BigNum: Equality - 2 != 5", "[pass]") {
  TEST_ASSERT((BigNum(2) == BigNum(5)) == false)
}

TEST_CASE("BigNum: Equality - 5 == 5", "[pass]") {
  TEST_ASSERT(BigNum(5) == BigNum(5))
}

TEST_CASE("BigNum: from_raw - 0x56 == 86", "[pass]") {
  char buf[2] = {0x56, 0};
  auto buf_s = string(buf);
  auto bn = BigNum::from_raw(buf_s);
  TEST_ASSERT(bn == 86);
}

TEST_CASE("BigNum: from_raw multi byte - 0x2935 == 10549", "[pass]") {
  char buf[3] = {0x29, 0x35, 0};
  auto buf_s = string(buf);
  auto bn = BigNum::from_raw(buf_s);
  TEST_ASSERT(bn == 10549);
}
