#include "cryptor.hpp"

#include <esp_log.h>
#include <sodium.h>

using namespace std;


Cryptor::Cryptor(const string& key, const string& nonce) : _key(key) {
  pad_and_set_nonce(nonce);
}


string Cryptor::encrypt(const string& plain_text) {
  return encrypt(plain_text, {});
}

optional<string> Cryptor::decrypt(const string& cypher_text) {
  return decrypt(cypher_text, {});
}

string Cryptor::encrypt(const string& plain_text, const string& aad) {
  string cypher_text(
    aad.size() + plain_text.size() + crypto_aead_chacha20poly1305_IETF_ABYTES,
    0
  );
  uint64_t cypher_text_length = 0;

  // Encrypt the plain text
  crypto_aead_chacha20poly1305_ietf_encrypt(
    (uint8_t*)&cypher_text[0],
    &cypher_text_length,
    (uint8_t*)plain_text.data(),
    plain_text.size(),
    (uint8_t*)aad.data(),
    aad.size(),
    NULL,
    (uint8_t*)_nonce.data(),
    (uint8_t*)_key.data()
  );

  // Resize the cypher text
  cypher_text.resize(cypher_text_length);

  return cypher_text;
}

optional<string> Cryptor::decrypt(const string& cypher_text, const std::string& aad) {
  string plain_text(cypher_text.size(), 0);

  // Auth and decrypt the cypher text
  uint64_t plain_text_length = 0;
  int res = crypto_aead_chacha20poly1305_ietf_decrypt(
    (uint8_t*)&plain_text[0],
    &plain_text_length,
    NULL,
    (uint8_t*)cypher_text.data(),
    cypher_text.size(),
    (uint8_t*)aad.data(),
    aad.size(),
    (uint8_t*)_nonce.data(),
    (uint8_t*)_key.data()
  );

  // Check if auth / decryption passed
  if(res != 0) {
    return {};
  }

  // Resize the plain text message
  plain_text.resize(plain_text_length);

  return {plain_text};
}

void Cryptor::set_nonce(const string& nonce) {
  pad_and_set_nonce(nonce);
}

void Cryptor::pad_and_set_nonce(const string& nonce) {
  _nonce = nonce;

  // Pad the nonce to 96 bits
  while(_nonce.size() < 12) { _nonce.insert(_nonce.begin(), 0); }
}
