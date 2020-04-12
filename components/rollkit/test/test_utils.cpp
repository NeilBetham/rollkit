#include "unity.h"
#include "rollkit/utils.hpp"

using namespace std;

TEST_CASE("Test to_hex 10 = 0x0a", "[pass]") {
  string bin_in = {10};
  string hex_out = "0A";

  auto hex_test = rollkit::to_hex(bin_in);

  TEST_ASSERT_EQUAL_STRING(hex_test.data(), hex_out.data());
}

TEST_CASE("Test to_hex {170, 170} == 0xaaaa", "[pass]") {
  string bin_in = {170, 170};
  string hex_out = "AAAA";

  auto hex_test = rollkit::to_hex(bin_in);

  TEST_ASSERT_EQUAL_STRING(hex_test.data(), hex_out.data());
}

TEST_CASE("Test to_hex {170, 160} == 0xaaa0", "[pass]") {
  string bin_in = {170, 160};
  string hex_out = "AAA0";

  auto hex_test = rollkit::to_hex(bin_in);

  TEST_ASSERT_EQUAL_STRING(hex_test.data(), hex_out.data());
}

TEST_CASE("Test hex_c_to_bin_c \"0xA\" == 0b1010", "[pass]") {
  TEST_ASSERT_EQUAL_INT(0b1010, rollkit::internal::hex_c_to_bin_c('a'));
  TEST_ASSERT_EQUAL_INT(0b1010, rollkit::internal::hex_c_to_bin_c('A'));
}

TEST_CASE("Test hex_c_to_bin_c \"0x5\" == 0b0101", "[pass]") {
  TEST_ASSERT_EQUAL_INT(0b0101, rollkit::internal::hex_c_to_bin_c('5'));
}

TEST_CASE("Test to_bin 0x35 == 53", "[pass]") {
  string hex_in = "35";
  string bin_out = {53};

  auto bin_test = rollkit::to_bin(hex_in);

  TEST_ASSERT_EQUAL_STRING(bin_out.data(), bin_test.data());
}

TEST_CASE("Test to_bin 0x3535 == \"55\"", "[pass]") {
  string hex_in = "3535";
  string bin_out = {53, 53};

  auto bin_test = rollkit::to_bin(hex_in);

  TEST_ASSERT_EQUAL_STRING(bin_out.data(), bin_test.data());
}

TEST_CASE("Test to_bin 0x3530 == \"50\"", "[pass]") {
  string hex_in = "3530";
  string bin_out = {53, 48};

  auto bin_test = rollkit::to_bin(hex_in);

  TEST_ASSERT_EQUAL_STRING(bin_out.data(), bin_test.data());
}
