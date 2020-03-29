#include "unity.h"
#include "session_security.hpp"

#include "utils.hpp"

using namespace std;

TEST_CASE("Test Decrypt Message", "[pass]") {
  string plain_text = "GET /accessories HTTP/1.1\r\nHost: 10.0.4.110:80\r\n\r\n";
  string aad     = to_bin("3200");
  string c2a_key = to_bin("F5318B2A934D94F6869C58DE352CA2CB03E5569CBEF5BB3A068723EFACECB827");
  string a2c_key = to_bin("55628A9855D81ED615A5BD70690ACC94651798D59FAB363387CBE2F107BB44AE");
  string cypher_text = to_bin("D2B01E2D3FA162BEAFACA7426077116B52CDACD78F6886E1E17CA5CE649F99E5019B282EBA6F8F97DF9ADDF90AB16978B42F");
  string tag_text = to_bin("A43EC8C2F6D49DE9195F48ACAA2621A1");

  SessionSecurity ss(a2c_key, c2a_key);
  auto decrypt_res = ss.decrypt(aad + cypher_text + tag_text);
  TEST_ASSERT(decrypt_res.has_value() != false);
  TEST_ASSERT_EQUAL_STRING(decrypt_res->c_str(), plain_text.c_str());
}
