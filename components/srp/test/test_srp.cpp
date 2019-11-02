#include <string>

#include "unity.h"
#include "srp.hpp"
#include "srp_test_vectors.hpp"

#include "esp_log.h"

using namespace std;

TEST_CASE("HomeKit SHA512 Hash Function", "[pass]") {
  auto b16_hash = BigNum::from_raw(SRP::External::sha512("hello")).export_b16();

  TEST_ASSERT_EQUAL_STRING(
    "9B71D224BD62F3785D96D46AD3EA3D73319BFBC2890CAADAE2DFF72519673CA72323C3D99BA5C11D7C7ACC6E14B8C5DA0C4663475C2E5C3ADEF46F73BCDEC043",
    b16_hash.c_str()
  );
}

TEST_CASE("HomeKit SRP Test Vectors - get_A", "[pass]") {
  TEST_ASSERT(SRP::get_A(tv_N(), tv_g(), tv_a()) == tv_A());
}

TEST_CASE("HomeKit SRP Test Vectors - get_v", "[pass]") {
  auto x = SRP::get_x("alice", "password123", tv_s());
  TEST_ASSERT(SRP::get_v(tv_N(), tv_g(), x) == tv_v());
}

TEST_CASE("HomeKit SRP Test Vectors - get_B", "[pass]") {
  TEST_ASSERT(SRP::get_B(tv_N(), tv_g(), tv_b(), SRP::get_k(tv_N(), tv_g()), tv_v()) == tv_B());
}

TEST_CASE("HomeKit SRP Test Vectors - get_u", "[pass]") {
  TEST_ASSERT(SRP::get_u(tv_N(), tv_A(), tv_B()) == tv_u());
}

TEST_CASE("HomeKit SRP Test Vectors - get_u", "[pass]") {
  TEST_ASSERT(SRP::get_u(tv_N(), tv_A(), tv_B()) == tv_u());
}
