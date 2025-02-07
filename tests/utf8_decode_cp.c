#include <unity.h>
#include "utf8.h"

/*********************************
 * utf8_codepoint_decode tests
 *********************************/
void test_utf8_decode_valid_ascii(void) {
    const uint8_t input[] = {0x41};
    uint32_t codepoint;
    size_t consumed;
    TEST_ASSERT_EQUAL(UTF8_OK, utf8_decode_cp(input, sizeof(input), &codepoint, &consumed));
    TEST_ASSERT_EQUAL(0x41, codepoint);
    TEST_ASSERT_EQUAL(1, consumed);
}

void test_utf8_decode_valid_4byte(void) {
    const uint8_t input[] = {0xF0, 0x9F, 0x98, 0x8A};
    uint32_t codepoint;
    size_t consumed;
    TEST_ASSERT_EQUAL(UTF8_OK, utf8_decode_cp(input, sizeof(input), &codepoint, &consumed));
    TEST_ASSERT_EQUAL(0x1F60A, codepoint);
    TEST_ASSERT_EQUAL(4, consumed);
}

void test_utf8_decode_invalid_sequence(void) {
    const uint8_t input[] = {0xC3}; // Incomplete sequence
    uint32_t codepoint;
    size_t consumed;
    TEST_ASSERT_EQUAL(UTF8_ERR_INVALID_SEQUENCE, utf8_decode_cp(input, sizeof(input), &codepoint, &consumed));
}

void test_utf8_decode_overlong_encoding(void) {
    const uint8_t input[] = {0xC0, 0xAF}; // Overlong '/'
    uint32_t codepoint;
    size_t consumed;
    TEST_ASSERT_EQUAL(UTF8_ERR_INVALID_SEQUENCE, utf8_decode_cp(input, sizeof(input), &codepoint, &consumed));
}

void test_utf8_decode_surrogate_pair(void) {
    const uint8_t input[] = {0xED, 0xA0, 0x80}; // Invalid surrogate
    uint32_t codepoint;
    size_t consumed;
    TEST_ASSERT_EQUAL(UTF8_ERR_INVALID_SEQUENCE, utf8_decode_cp(input, sizeof(input), &codepoint, &consumed));
}
