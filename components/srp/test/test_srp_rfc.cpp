#include <string>

#include "unity.h"
#include "rfc_test_vectors.hpp"
#include "sha1.hpp"
#include "srp.hpp"

#include "esp_log.h"

using namespace std;

static SHA1 hash_fn;
static SRP::Math math(hash_fn, tv_N(), tv_g());


TEST_CASE("RFC 5054 - SHA1 Hash Function", "[PASS]") {
  auto b16_hash = BigNum::from_raw(hash_fn.hash("hello")).export_b16();

  TEST_ASSERT_EQUAL_STRING(
    "AAF4C61DDCC5E8A2DABEDE0F3B482CD9AEA9434D",
    b16_hash.c_str()
  );
}

TEST_CASE("RFC 5054 Test Vectors - get_k", "[PASS]") {
  TEST_ASSERT(math.get_k() == tv_k());
}

TEST_CASE("RFC 5054 Test Vectors - get_x", "[PASS]") {
  TEST_ASSERT(math.get_x("alice", "password123", tv_s()) == tv_x());
}

TEST_CASE("RFC 5054 Test Vectors - get_A", "[PASS]") {
  TEST_ASSERT(math.get_A(tv_a()) == tv_A());
}

TEST_CASE("RFC 5054 Test Vectors - get_v", "[PASS]") {
  TEST_ASSERT(math.get_v(math.get_x("alice", "password123", tv_s())) == tv_v());
}

TEST_CASE("RFC 5054 Test Vectors - get_B", "[PASS]") {
  TEST_ASSERT(math.get_B(tv_b(), tv_k(), tv_v()) == tv_B());
}

TEST_CASE("RFC 5054 Test Vectors - get_u", "[PASS]") {
  TEST_ASSERT(math.get_u(tv_A(), tv_B()) == tv_u());
}

TEST_CASE("RFC 5054 Test Vectors - get_S", "[PASS]") {
  TEST_ASSERT(math.get_S_host(tv_A(), tv_v(), tv_u(), tv_b()) == tv_S());
}
