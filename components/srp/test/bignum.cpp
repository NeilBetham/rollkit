#include "unity.h"
#include "srp.hpp"

TEST_CASE("BigNum: Equality - 2 != 5", "[pass]") {
  TEST_ASSERT((BigNum(2) == BigNum(5)) == false)
}

TEST_CASE("BigNum: Equality - 5 == 5", "[pass]") {
  TEST_ASSERT(BigNum(5) == BigNum(5))
}
