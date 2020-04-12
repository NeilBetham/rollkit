#include "unity.h"
#include "rollkit/curve25519.hpp"
#include "rollkit/utils.hpp"

using namespace std;

const string alice_private_key = rollkit::to_bin("77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a");
const string alice_public_key  = rollkit::to_bin("8520f0098930a754748b7ddcb43ef75a0dbf3a0d26381af4eba4a98eaa9b4e6a");
const string bob_private_key   = rollkit::to_bin("5dab087e624a8a4b79e17f8b83800ee66f3bb1292618b6fd1c2f8b27ff88e0eb");
const string bob_public_key    = rollkit::to_bin("de9edb7d7b7dc1b4d35b61c2ece435373f8343c85b78674dadfc7e146f882b4f");

const string shared_secret     = rollkit::to_bin("4a5d9d5ba4ce2de1728e3bf480350f25e07e21c947d19e3376f09b3c1e161742");

TEST_CASE("Test Curve25519 Alice to Bob Shared Secret ", "[pass]") {
  auto test_shared_secret = rollkit::Curve25519::compute_shared_secret(alice_private_key, bob_public_key);
  TEST_ASSERT_EQUAL_STRING(shared_secret.c_str(), test_shared_secret.c_str());
}

TEST_CASE("Test Curve25519 Bob to Alice Shared Secret ", "[pass]") {
  auto test_shared_secret = rollkit::Curve25519::compute_shared_secret(bob_private_key, alice_public_key);
  TEST_ASSERT_EQUAL_STRING(shared_secret.c_str(), test_shared_secret.c_str());
}

TEST_CASE("Test Curve25519 HomeKit Example", "[pass]") {
  auto ios_private_key = rollkit::to_bin("68a630e6676dcf0e5afed17fe3baaa158fe1230b659e1c92dea40389efba4364");
  auto acc_public_key  = rollkit::to_bin("d9992a873d1afee1522d7b353ea7284b783cb5f51a405fb4eb3054ee2e1ff54c");
  auto shared_secret   = rollkit::to_bin("9366e8c67c64c9a17bf6ddf66a6440be6d842f94eb8724e89761c24c969f8912");

  auto test_shared_secret = rollkit::Curve25519::compute_shared_secret(ios_private_key, acc_public_key);
  TEST_ASSERT_EQUAL_STRING(shared_secret.c_str(), test_shared_secret.c_str());
}
