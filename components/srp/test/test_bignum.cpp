#include "unity.h"
#include "srp.hpp"


using namespace std;

TEST_CASE("BigNum: Equality - 2 != 5", "[pass]") {
  TEST_ASSERT((BigNum(2) == BigNum(5)) == false)
}

TEST_CASE("BigNum: Equality - 5 == 5", "[pass]") {
  TEST_ASSERT(BigNum(5) == BigNum(5))
}

TEST_CASE("BigNum: Addition - 5 + 305 == 310", "[pass]") {
  TEST_ASSERT(BigNum(5) + BigNum(305) == BigNum(310))
}

TEST_CASE("BigNum: Multiplication - 20 * 20 == 400", "[pass]") {
  TEST_ASSERT(BigNum(20) * BigNum(20) == BigNum(400))
}

TEST_CASE("BigNum: Multiplication - k * v == k*v", "[pass]") {
  TEST_ASSERT(BigNum("7556AA045AEF2CDD07ABAF0F665C3E818913186F") *
  BigNum("\
7E273DE8696FFC4F4E337D05B4B375BEB0DDE1569E8FA00A9886D8129BADA1\
F1822223CA1A605B530E379BA4729FDC59F105B4787E5186F5C671085A1447\
B52A48CF1970B4FB6F8400BBF4CEBFBB168152E08AB5EA53D15C1AFF87B2B9\
DA6E04E058AD51CC72BFC9033B564E26480D78E955A5E29E7AB245DB2BE315\
E2099AFB\
") == BigNum("\
39D2A44238CC9017300647C9D9171B1468AE0A4C84F6AF97B31CD3D3885C\
1B339CEB107E5ED1CC92A691BB0D793C0490FDC35F114DD118E61A43A11C\
E6CE818D33A055DC3BF968E602D2B87D729D375E246BD3EFF39FBB4C3EE0\
017F2F6DC4994EB607D827909A1FC34D4B0D49D0A8FF88F51452EE304F0E\
A1C5A18FEC326813942DBFB11B9E0B4C6D980D4CA47C50CFBC52BAD5\
"))
}

TEST_CASE("BigNum: mod - 10 % 5 == 0", "[pass]") {
  TEST_ASSERT(BigNum(10) % BigNum(5) == 0);
}

TEST_CASE("BigNum: mod - 5 % 10 == 5", "[pass]") {
  TEST_ASSERT(BigNum(5) % BigNum(10) == 5);
}

TEST_CASE("BigNum: from_raw - 0x56 == 86", "[pass]") {
  char buf[2] = {0x56, 0};
  auto buf_s = string(buf);
  auto bn = BigNum::from_raw(buf_s);
  TEST_ASSERT(bn == 86);
}

TEST_CASE("BigNum: export_raw - 86 == 0x56", "[pass]") {
  char buf[2] = {0x56, 0};
  auto buf_s = string(buf);
  auto bn = BigNum(86);
  TEST_ASSERT(bn.export_raw() == buf_s);
}

TEST_CASE("BigNum: from_raw multi byte - 0x2935 == 10549", "[pass]") {
  char buf[3] = {0x29, 0x35, 0};
  auto buf_s = string(buf);
  auto bn = BigNum::from_raw(buf_s);
  TEST_ASSERT(bn == 10549);
}

TEST_CASE("BigNum: export_raw multi byte - 16706 == 0x4142", "[pass]") {
  char buf[3] = {0x41, 0x42, 0};
  auto buf_s = string(buf);
  auto bn = BigNum(16706);
  TEST_ASSERT(bn.export_raw() == buf_s);
}
