#include "unity.h"
#include "hkdf.hpp"

using namespace std;

TEST_CASE("Test SHA512 HKDF Test Vectors A", "[pass]") {
  string ikm = "0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b";
  string salt = "000102030405060708090a0b0c";
  string info = "f0f1f2f3f4f5f6f7f8f9";
  int l = 42;
  string okm = "832390086cda71fb47625bb5ceb168e4c8e26a1a16ed34d9fc7fe92c1481579338da362cb8d9f925d7cb";

  TEST_ASSERT(hkdf_sha512(ikm, salt, info, l) == okm);
}
