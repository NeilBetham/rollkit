#include "unity.h"
#include "cryptor.hpp"

#include <sodium.h>

using namespace std;

string test_key(crypto_aead_chacha20poly1305_IETF_KEYBYTES, 0);
string test_nonce = {0, 0, 0, 0, 'P', 'S', '-', 'M', 's', 's', '0', '5'};

TEST_CASE("Test Encrypt Followed By Decrypt", "[pass]") {
  Cryptor cryptor(test_key, test_nonce);
  string message = "Hello World!";
  auto cypher_text = cryptor.encrypt(message);
  auto plain_text = cryptor.decrypt(cypher_text);
  TEST_ASSERT(plain_text.has_value() != false);
  TEST_ASSERT(*plain_text == message);
}

TEST_CASE("Test AAD Encrypt Followed By Decrypt", "[pass]") {
  Cryptor cryptor(test_key, test_nonce);
  string message = "Hello World!";
  uint16_t message_size = message.size();
  string aad((char*)&message_size, 2);
  auto cypher_text = cryptor.encrypt(message, aad);
  auto plain_text = cryptor.decrypt(cypher_text, aad);
  TEST_ASSERT(plain_text.has_value() != false);
  TEST_ASSERT(*plain_text == message);
}
