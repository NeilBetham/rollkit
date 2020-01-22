#ifndef SRP_TEST_VECTORS_HPP
#define SRP_TEST_VECTORS_HPP

#include "srp/bignum.hpp"

// RFC Modulus
static BigNum tv_N() {
  return BigNum("\
FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74\
020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F1437\
4FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED\
EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF05\
98DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB\
9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3B\
E39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF695581718\
3995497CEA956AE515D2261898FA051015728E5A8AAAC42DAD33170D04507A33\
A85521ABDF1CBA64ECFB850458DBEF0A8AEA71575D060C7DB3970F85A6E1E4C7\
ABF5AE8CDB0933D71E8C94E04A25619DCEE3D2261AD2EE6BF12FFA06D98A0864\
D87602733EC86A64521F2B18177B200CBBE117577A615D6C770988C0BAD946E2\
08E24FA074E5AB3143DB5BFCE0FD108E4B82D120A93AD2CAFFFFFFFFFFFFFFFF\
");
}

// RFC Generator
static BigNum tv_g() {
  return BigNum("5");
}

// A side Private Key
static BigNum tv_a() {
  return BigNum("\
60975527035CF2AD1989806F0407210BC81EDC04E2762A56AFD529DDDA2D4393\
");
}

// A side Public Key
static BigNum tv_A() {
  return BigNum("\
FAB6F5D2615D1E323512E7991CC37443F487DA604CA8C9230FCB04E541DCE628\
0B27CA4680B0374F179DC3BDC7553FE62459798C701AD864A91390A28C93B644\
ADBF9C00745B942B79F9012A21B9B78782319D83A1F8362866FBD6F46BFC0DDB\
2E1AB6E4B45A9906B82E37F05D6F97F6A3EB6E182079759C4F6847837B62321A\
C1B4FA68641FCB4BB98DD697A0C73641385F4BAB25B793584CC39FC8D48D4BD8\
67A9A3C10F8EA12170268E34FE3BBE6FF89998D60DA2F3E4283CBEC1393D52AF\
724A57230C604E9FBCE583D7613E6BFFD67596AD121A8707EEC4694495703368\
6A155F644D5C5863B48F61BDBF19A53EAB6DAD0A186B8C152E5F5D8CAD4B0EF8\
AA4EA5008834C3CD342E5E0F167AD04592CD8BD279639398EF9E114DFAAAB919\
E14E850989224DDD98576D79385D2210902E9F9B1F2D86CFA47EE244635465F7\
1058421A0184BE51DD10CC9D079E6F1604E7AA9B7CF7883C7D4CE12B06EBE160\
81E23F27A231D18432D7D1BB55C28AE21FFCF005F57528D15A88881BB3BBB7FE\
");
}

// B side Private Key
static BigNum tv_b() {
  return BigNum("\
E487CB59D31AC550471E81F00F6928E01DDA08E974A004F49E61F5D105284D20\
");
}

// B side Public Key
static BigNum tv_B() {
  return BigNum("\
40F57088A482D4C7733384FE0D301FDDCA9080AD7D4F6FDF09A01006C3CB6D56\
2E41639AE8FA21DE3B5DBA7585B275589BDB279863C562807B2B99083CD1429C\
DBE89E25BFBD7E3CAD3173B2E3C5A0B174DA6D5391E6A06E465F037A40062548\
39A56BF76DA84B1C94E0AE208576156FE5C140A4BA4FFC9E38C3B07B88845FC6\
F7DDDA93381FE0CA6084C4CD2D336E5451C464CCB6EC65E7D16E548A273E8262\
84AF2559B6264274215960FFF47BDD63D3AFF064D6137AF769661C9D4FEE4738\
2603C88EAA0980581D07758461B777E4356DDA5835198B51FEEA308D70F75450\
B71675C08C7D8302FD7539DD1FF2A11CB4258AA70D234436AA42B6A0615F3F91\
5D55CC3B966B2716B36E4D1A06CE5E5D2EA3BEE5A1270E8751DA45B60B997B0F\
FDB0F9962FEE4F03BEE780BA0A845B1D9271421783AE6601A61EA2E342E4F2E8\
BC935A409EAD19F221BD1B74E2964DD19FC845F60EFC09338B60B6B256D8CAC8\
89CCA306CC370A0B18C8B886E95DA0AF5235FEF4393020D2B7F3056904759042\
");
}

// Salt
static BigNum tv_s() {
  return BigNum("BEB25379D1A8581EB5A727673A2441EE");
}

// Verifier
static BigNum tv_v() {
  return BigNum("\
9B5E061701EA7AEB39CF6E3519655A853CF94C75CAF2555EF1FAF759BB79CB47\
7014E04A88D68FFC05323891D4C205B8DE81C2F203D8FAD1B24D2C109737F1BE\
BBD71F912447C4A03C26B9FAD8EDB3E780778E302529ED1EE138CCFC36D4BA31\
3CC48B14EA8C22A0186B222E655F2DF5603FD75DF76B3B08FF8950069ADD03A7\
54EE4AE88587CCE1BFDE36794DBAE4592B7B904F442B041CB17AEBAD1E3AEBE3\
CBE99DE65F4BB1FA00B0E7AF06863DB53B02254EC66E781E3B62A8212C86BEB0\
D50B5BA6D0B478D8C4E9BBCEC21765326FBD14058D2BBDE2C33045F03873E539\
48D78B794F0790E48C36AED6E880F557427B2FC06DB5E1E2E1D7E661AC482D18\
E528D7295EF7437295FF1A72D402771713F16876DD050AE5B7AD53CCB90855C9\
3956648358ADFD966422F52498732D68D1D7FBEF10D78034AB8DCB6F0FCF885C\
C2B2EA2C3E6AC86609EA058A9DA8CC63531DC915414DF568B09482DDAC1954DE\
C7EB714F6FF7D44CD5B86F6BD115810930637C01D0F6013BC9740FA2C633BA89\
");
}

// Random Scrambling Parameter
static BigNum tv_u() {
  return BigNum("\
03AE5F3C3FA9EFF1A50D7DBB8D2F60A1EA66EA712D50AE976EE34641A1CD0E51\
C4683DA383E8595D6CB56A15D5FBC7543E07FBDDD316217E01A391A18EF06DFF\
");
}

// Pre Master Secret
static BigNum tv_S() {
  return BigNum("\
F1036FECD017C8239C0D5AF7E0FCF0D408B009E36411618A60B23AABBFC38339\
7268231214BAACDC94CA1C53F442FB51C1B027C318AE238E16414D60D1881B66\
486ADE10ED02BA33D098F6CE9BCF1BB0C46CA2C47F2F174C59A9C61E2560899B\
83EF61131E6FB30B714F4E43B735C9FE6080477C1B83E4093E4D456B9BCA492C\
F9339D45BC42E67CE6C02C243E49F5DA42A869EC855780E84207B8A1EA6501C4\
78AAC0DFD3D22614F531A00D826B7954AE8B14A985A429315E6DD3664CF47181\
496A94329CDE8005CAE63C2F9CA4969BFE84001924037C446559BDBB9DB9D4DD\
142FBCD75EEF2E162C843065D99E8F05762C4DB7ABD9DB203D41AC85A58C05BD\
4E2DBF822A934523D54E0653D376CE8B56DCB4527DDDC1B994DC7509463A7468\
D7F02B1BEB1685714CE1DD1E71808A137F788847B7C6B7BFA1364474B3B7E894\
78954F6A8E68D45B85A88E4EBFEC13368EC0891C3BC86CF50097880178D86135\
E728723458538858D715B7B247406222C1019F53603F016952D497100858824C\
");
}

// Session Key
static BigNum tv_K() {
  return BigNum("\
5CBC219DB052138EE1148C71CD4498963D682549CE91CA24F098468F06015BEB\
6AF245C2093F98C3651BCA83AB8CAB2B580BBF02184FEFDF26142F73DF95AC50\
");
}



#endif // SRP_TEST_VECTORS_HPP
