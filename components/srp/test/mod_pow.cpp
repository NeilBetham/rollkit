#include "unity.h"
#include "srp.hpp"






TEST_CASE("Modular Exponentiation 2^6%5", "[pass]") {
  TEST_ASSERT(SRP::mod_pow({2}, {6}, {5}) == BigInt(4));
}

TEST_CASE("RFC 5054 Test Vector: A=g^a%N", "[pass]"){
  static BigInt modulus = SRP::to_bigint("\
  FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E08\
  8A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B\
  302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9\
  A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE6\
  49286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8\
  FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D\
  670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C\
  180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF695581718\
  3995497CEA956AE515D2261898FA051015728E5A8AAAC42DAD33170D\
  04507A33A85521ABDF1CBA64ECFB850458DBEF0A8AEA71575D060C7D\
  B3970F85A6E1E4C7ABF5AE8CDB0933D71E8C94E04A25619DCEE3D226\
  1AD2EE6BF12FFA06D98A0864D87602733EC86A64521F2B18177B200C\
  BBE117577A615D6C770988C0BAD946E208E24FA074E5AB3143DB5BFC\
  E0FD108E4B82D120A93AD2CAFFFFFFFFFFFFFFFF\
  ");

  static BigInt base("5");

  static BigInt exponent = SRP::to_bigint("\
  60975527035CF2AD1989806F0407210BC81EDC04E2762A56AFD529DDDA2D4393\
  ");

  static BigInt result = SRP::to_bigint("\
  61D5E490F6F1B79547B0704C436F523DD0E560F0C64115BB72557EC4\
  4352E8903211C04692272D8B2D1A5358A2CF1B6E0BFCF99F921530EC\
  8E39356179EAE45E42BA92AEACED825171E1E8B9AF6D9C03E1327F44\
  BE087EF06530E69F66615261EEF54073CA11CF5858F0EDFDFE15EFEA\
  B349EF5D76988A3672FAC47B0769447B\
  ");
  
  TEST_ASSERT(SRP::mod_pow(base, exponent, modulus) == result);
}