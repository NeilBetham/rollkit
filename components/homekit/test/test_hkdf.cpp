#include "unity.h"
#include "utils.hpp"
#include "hkdf.hpp"

#include "esp_log.h"

using namespace std;

// https://tools.ietf.org/html/rfc4231

TEST_CASE("Test Sha512 HMAC Test Case 1", "[pass]") {
  auto key = to_bin("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b");
  auto data = to_bin("4869205468657265");
  auto mac = to_bin("87aa7cdea5ef619d4ff0b4241a1d6cb02379f4e2ce4ec2787ad0b30545e17cdedaa833b7d6b8a702038b274eaea3f4e4be9d914eeb61f1702e696c203a126854");

  auto test_mac = internal::hmac_sha512(key, data);

  TEST_ASSERT_EQUAL_STRING(mac.data(), test_mac.data());
}

TEST_CASE("Test Sha512 HMAC Test Case 2", "[pass]") {
  auto key = to_bin("4a656665");
  auto data = to_bin("7768617420646f2079612077616e7420666f72206e6f7468696e673f");
  auto mac = to_bin("164b7a7bfcf819e2e395fbe73b56e0a387bd64222e831fd610270cd7ea2505549758bf75c05a994a6d034f65f8f0e6fdcaeab1a34d4a6b4b636e070a38bce737");

  auto test_mac = internal::hmac_sha512(key, data);

  TEST_ASSERT_EQUAL_STRING(mac.data(), test_mac.data());
}

TEST_CASE("Test Sha512 HMAC Test Case 3", "[pass]") {
  auto key = to_bin("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  auto data = to_bin("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd");
  auto mac = to_bin("fa73b0089d56a284efb0f0756c890be9b1b5dbdd8ee81a3655f83e33b2279d39bf3e848279a722c806b485a47e67c807b946a337bee8942674278859e13292fb");

  auto test_mac = internal::hmac_sha512(key, data);

  TEST_ASSERT_EQUAL_STRING(mac.data(), test_mac.data());
}

TEST_CASE("Test Sha512 HMAC Test Case 4", "[pass]") {
  auto key = to_bin("0102030405060708090a0b0c0d0e0f10111213141516171819");
  auto data = to_bin("cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd");
  auto mac = to_bin("b0ba465637458c6990e5a8c5f61d4af7e576d97ff94b872de76f8050361ee3dba91ca5c11aa25eb4d679275cc5788063a5f19741120c4f2de2adebeb10a298dd");

  auto test_mac = internal::hmac_sha512(key, data);

  TEST_ASSERT_EQUAL_STRING(mac.data(), test_mac.data());
}

TEST_CASE("Test Sha512 HMAC Test Case 5", "[pass]") {
  auto key = to_bin("0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c");
  auto data = to_bin("546573742057697468205472756e636174696f6e");
  auto mac = to_bin("415fad6271580a531d4179bc891d87a6");

  auto test_mac = internal::hmac_sha512(key, data);
  test_mac.resize(16);

  TEST_ASSERT_EQUAL_STRING(mac.data(), test_mac.data());
}

TEST_CASE("Test Sha512 HMAC Test Case 6", "[pass]") {
  auto key = to_bin("\
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
aaaaaa\
");
  auto data = to_bin("54657374205573696e67204c6172676572205468616e20426c6f636b2d53697a65204b6579202d2048617368204b6579204669727374");
  auto mac = to_bin("80b24263c7c1a3ebb71493c1dd7be8b49b46d1f41b4aeec1121b013783f8f3526b56d037e05f2598bd0fd2215d6a1e5295e64f73f63f0aec8b915a985d786598");

  auto test_mac = internal::hmac_sha512(key, data);

  TEST_ASSERT_EQUAL_STRING(mac.data(), test_mac.data());
}

TEST_CASE("Test Sha512 HMAC Test Case 7", "[pass]") {
  auto key = to_bin("\
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
aaaaaa\
");
  auto data = to_bin("\
5468697320697320612074657374207573696e672061206c6172676572207468\
616e20626c6f636b2d73697a65206b657920616e642061206c61726765722074\
68616e20626c6f636b2d73697a6520646174612e20546865206b6579206e6565\
647320746f20626520686173686564206265666f7265206265696e6720757365\
642062792074686520484d414320616c676f726974686d2e\
");
  auto mac = to_bin("\
e37b6a775dc87dbaa4dfa9f96e5e3ffddebd71f8867289865df5a32d20cdc944\
b6022cac3c4982b10d5eeb55c3e4de15134676fb6de0446065c97440fa8c6a58\
");

  auto test_mac = internal::hmac_sha512(key, data);

  TEST_ASSERT_EQUAL_STRING(mac.data(), test_mac.data());
}

TEST_CASE("Test SHA512 HKDF Test Vectors A", "[pass]") {
  auto ikm  = to_bin("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b");
  auto salt = to_bin("000102030405060708090a0b0c");
  auto info = to_bin("f0f1f2f3f4f5f6f7f8f9");
  int l     = 42;
  auto prk  = to_bin("665799823737ded04a88e47e54a5890bb2c3d247c7a4254a8e61350723590a26c36238127d8661b88cf80ef802d57e2f7cebcf1e00e083848be19929c61b4237");
  auto okm  = to_bin("832390086cda71fb47625bb5ceb168e4c8e26a1a16ed34d9fc7fe92c1481579338da362cb8d9f925d7cb");

  auto test_prk = internal::extract(salt, ikm);
  auto hkdf = hkdf_sha512(ikm, salt, info, l);

  TEST_ASSERT_EQUAL_INT(test_prk.size(), prk.size());
  TEST_ASSERT_EQUAL_STRING(test_prk.data(), prk.data());
  TEST_ASSERT_EQUAL_INT(hkdf.size(), l);
  TEST_ASSERT_EQUAL_STRING(hkdf.data(), okm.data());
}

TEST_CASE("Test SHA512 HKDF Test Vectors B", "[pass]") {
  auto ikm  = to_bin("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f");
  auto salt = to_bin("606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeaf");
  auto info = to_bin("b0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff");
  int l = 82;
  auto prk  = to_bin("35672542907d4e142c00e84499e74e1de08be86535f924e022804ad775dde27ec86cd1e5b7d178c74489bdbeb30712beb82d4f97416c5a94ea81ebdf3e629e4a");
  auto okm  = to_bin("ce6c97192805b346e6161e821ed165673b84f400a2b514b2fe23d84cd189ddf1b695b48cbd1c8388441137b3ce28f16aa64ba33ba466b24df6cfcb021ecff235f6a2056ce3af1de44d572097a8505d9e7a93");

  auto test_prk = internal::extract(salt, ikm);
  auto hkdf = hkdf_sha512(ikm, salt, info, l);

  TEST_ASSERT_EQUAL_INT(test_prk.size(), prk.size());
  TEST_ASSERT_EQUAL_STRING(test_prk.data(), prk.data());
  TEST_ASSERT_EQUAL_INT(hkdf.size(), l);
  TEST_ASSERT_EQUAL_STRING(hkdf.data(), okm.data());
}

TEST_CASE("Test SHA512 HKDF Test Vectors C", "[pass]") {
  auto ikm  = to_bin("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b");
  auto salt = to_bin("");
  auto info = to_bin("");
  int l = 42;
  auto prk  = to_bin("fd200c4987ac491313bd4a2a13287121247239e11c9ef82802044b66ef357e5b194498d0682611382348572a7b1611de54764094286320578a863f36562b0df6");
  auto okm  = to_bin("f5fa02b18298a72a8c23898a8703472c6eb179dc204c03425c970e3b164bf90fff22d04836d0e2343bac");

  auto test_prk = internal::extract(salt, ikm);
  auto hkdf = hkdf_sha512(ikm, salt, info, l);

  TEST_ASSERT_EQUAL_INT(test_prk.size(), prk.size());
  TEST_ASSERT_EQUAL_STRING(test_prk.data(), prk.data());
  TEST_ASSERT_EQUAL_INT(hkdf.size(), l);
  TEST_ASSERT_EQUAL_STRING(hkdf.data(), okm.data());
}

TEST_CASE("Test SHA512 HKDF Test Vectors D", "[pass]") {
  auto ikm  = to_bin("0b0b0b0b0b0b0b0b0b0b0b");
  auto salt = to_bin("000102030405060708090a0b0c");
  auto info = to_bin("f0f1f2f3f4f5f6f7f8f9");
  int l = 42;
  auto prk  = to_bin("67409c9cac28b52ee9fad91c2fda999f7ca22e3434f0ae772863836568ad6a7f10cf113bfddd560129a594a8f52385c2d661d785d29ce93a11400c920683181d");
  auto okm  = to_bin("7413e8997e020610fbf6823f2ce14bff01875db1ca55f68cfcf3954dc8aff53559bd5e3028b080f7c068");

  auto test_prk = internal::extract(salt, ikm);
  auto hkdf = hkdf_sha512(ikm, salt, info, l);

  TEST_ASSERT_EQUAL_INT(test_prk.size(), prk.size());
  TEST_ASSERT_EQUAL_STRING(test_prk.data(), prk.data());
  TEST_ASSERT_EQUAL_INT(hkdf.size(), l);
  TEST_ASSERT_EQUAL_STRING(hkdf.data(), okm.data());
}
