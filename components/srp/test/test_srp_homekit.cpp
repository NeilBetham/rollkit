#include <string>

#include "unity.h"
#include "srp.hpp"
#include "srp_test_vectors.hpp"
#include "sha512.hpp"

#include "esp_log.h"

using namespace std;

static SHA512 hash_fn;
static SRP::Math math(hash_fn, tv_N(), tv_g());

TEST_CASE("HomeKit - SHA512 Hash Function", "[pass]") {
  auto b16_hash = BigNum::from_raw(hash_fn.hash("hello")).export_b16();

  TEST_ASSERT_EQUAL_STRING(
    "9B71D224BD62F3785D96D46AD3EA3D73319BFBC2890CAADAE2DFF72519673CA72323C3D99BA5C11D7C7ACC6E14B8C5DA0C4663475C2E5C3ADEF46F73BCDEC043",
    b16_hash.c_str()
  );
}

TEST_CASE("HomeKit SRP Test Vectors - get_A", "[pass]") {
  TEST_ASSERT(math.get_A(tv_a()) == tv_A());
}

TEST_CASE("HomeKit SRP Test Vectors - get_v", "[pass]") {
  TEST_ASSERT(math.get_v(math.get_x("alice", "password123", tv_s())) == tv_v());
}

TEST_CASE("HomeKit SRP Test Vectors - get_B", "[pass]") {
  TEST_ASSERT(math.get_B(tv_b(), math.get_k(), tv_v()) == tv_B());
}

TEST_CASE("HomeKit SRP Test Vectors - get_u", "[pass]") {
  TEST_ASSERT(math.get_u(tv_A(), tv_B()) == tv_u());
}
