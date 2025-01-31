#include <unity.h>

void setUp(void) {
}

void tearDown(void) {
}

/*********************************
 * utf8_str_validate tests
 *********************************/
extern void test_utf8_str_validate_valid(void);

extern void test_utf8_str_validate_incomplete_sequence(void);

extern void test_utf8_str_validate_null_input(void);

extern void test_utf8_str_validate_overlong_encoding(void);

/*********************************
 * utf8_str_count_codepoints tests
 *********************************/
extern void test_utf8_str_count_valid(void);

extern void test_utf8_str_count_invalid_sequence(void);

extern void test_utf8_str_count_null_input(void);

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

 // Validation tests
 RUN_TEST(test_utf8_str_validate_valid);
 RUN_TEST(test_utf8_str_validate_incomplete_sequence);
 RUN_TEST(test_utf8_str_validate_null_input);
 RUN_TEST(test_utf8_str_validate_overlong_encoding);

 // Count codepoints tests
 RUN_TEST(test_utf8_str_count_valid);
 RUN_TEST(test_utf8_str_count_invalid_sequence);
 RUN_TEST(test_utf8_str_count_null_input);

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
