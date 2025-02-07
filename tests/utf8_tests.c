#include <unity.h>

void setUp(void) {
}

void tearDown(void) {
}

/*********************************
 * utf8_codepoint_encode tests
 *********************************/
extern void test_utf8_encode_valid_ascii(void);

extern void test_utf8_encode_valid_4byte(void);

extern void test_utf8_encode_invalid_codepoint(void);

extern void test_utf8_encode_buffer_too_small(void);


/*********************************
 * utf8_codepoint_decode tests
 *********************************/
extern void test_utf8_decode_valid_ascii(void);

extern void test_utf8_decode_valid_4byte(void);

extern void test_utf8_decode_invalid_sequence(void);

extern void test_utf8_decode_overlong_encoding(void);

extern void test_utf8_decode_surrogate_pair(void);

/*********************************
 * Test Runner
 *********************************/
int main(void) {
 UNITY_BEGIN();

 // Encode tests
 RUN_TEST(test_utf8_encode_valid_ascii);
 RUN_TEST(test_utf8_encode_valid_4byte);
 RUN_TEST(test_utf8_encode_invalid_codepoint);
 RUN_TEST(test_utf8_encode_buffer_too_small);

 // Decode tests
 RUN_TEST(test_utf8_decode_valid_ascii);
 RUN_TEST(test_utf8_decode_valid_4byte);
 RUN_TEST(test_utf8_decode_invalid_sequence);
 RUN_TEST(test_utf8_decode_overlong_encoding);
 RUN_TEST(test_utf8_decode_surrogate_pair);

 return UNITY_END();
}
