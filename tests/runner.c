#include "unity.h"
#include "utf8.h"
#include <stdint.h>
#include <stddef.h>

void setUp(void) {
}

void tearDown(void) {
}

// Helper function for counting codepoints in tests
static void count_callback(uint32_t codepoint, void *context) {
    (void) codepoint;
    (*(size_t *) context)++;
}

/*********************************
 * utf8_str_validate tests
 *********************************/
void test_utf8_str_validate_valid(void) {
    const uint8_t valid_str[] = {0xC3, 0xA9, 0x6E, 0x64}; // "énd"
    TEST_ASSERT_EQUAL(UTF8_OK, utf8_str_validate(valid_str, sizeof(valid_str)));
}

void test_utf8_str_validate_incomplete_sequence(void) {
    const uint8_t invalid_str[] = {0xC3}; // Missing continuation byte
    TEST_ASSERT_EQUAL(UTF8_ERR_INVALID_SEQUENCE, utf8_str_validate(invalid_str, sizeof(invalid_str)));
}

void test_utf8_str_validate_null_input(void) {
    TEST_ASSERT_EQUAL(UTF8_ERR_NULL_POINTER, utf8_str_validate(NULL, 4));
}

void test_utf8_str_validate_overlong_encoding(void) {
    const uint8_t overlong[] = {0xC0, 0xAF}; // Overlong encoding for '/'
    TEST_ASSERT_EQUAL(UTF8_ERR_INVALID_SEQUENCE, utf8_str_validate(overlong, sizeof(overlong)));
}

/*********************************
 * utf8_str_count_codepoints tests
 *********************************/
void test_utf8_str_count_valid(void) {
    const uint8_t str[] = {0xF0, 0x9F, 0x98, 0x8A, 0x41}; // 😊A
    size_t count;
    TEST_ASSERT_EQUAL(UTF8_OK, utf8_str_count_codepoints(str, sizeof(str), &count));
    TEST_ASSERT_EQUAL(2, count);
}

void test_utf8_str_count_invalid_sequence(void) {
    const uint8_t invalid_str[] = {0xED, 0xA0, 0x80}; // Invalid surrogate
    size_t count;
    TEST_ASSERT_EQUAL(UTF8_ERR_INVALID_SEQUENCE, utf8_str_count_codepoints(invalid_str, sizeof(invalid_str), &count));
}

void test_utf8_str_count_null_input(void) {
    size_t count;
    TEST_ASSERT_EQUAL(UTF8_ERR_NULL_POINTER, utf8_str_count_codepoints(NULL, 4, &count));
    TEST_ASSERT_EQUAL(UTF8_ERR_NULL_POINTER, utf8_str_count_codepoints((uint8_t[]){0x41}, 1, NULL));
}

/*********************************
 * utf8_codepoint_encode tests
 *********************************/
void test_utf8_encode_valid_ascii(void) {
    uint8_t buffer[4];
    size_t written;
    TEST_ASSERT_EQUAL(UTF8_OK, utf8_codepoint_encode(0x41, buffer, sizeof(buffer), &written));
    TEST_ASSERT_EQUAL(1, written);
    TEST_ASSERT_EQUAL(0x41, buffer[0]);
}

void test_utf8_encode_valid_4byte(void) {
    uint8_t buffer[4];
    size_t written;
    TEST_ASSERT_EQUAL(UTF8_OK, utf8_codepoint_encode(0x1F60A, buffer, sizeof(buffer), &written));
    TEST_ASSERT_EQUAL(4, written);
    TEST_ASSERT_EQUAL(0xF0, buffer[0]);
    TEST_ASSERT_EQUAL(0x9F, buffer[1]);
    TEST_ASSERT_EQUAL(0x98, buffer[2]);
    TEST_ASSERT_EQUAL(0x8A, buffer[3]);
}

void test_utf8_encode_invalid_codepoint(void) {
    uint8_t buffer[4];
    size_t written;
    TEST_ASSERT_EQUAL(UTF8_ERR_INVALID_CODEPOINT, utf8_codepoint_encode(0x110000, buffer, sizeof(buffer), &written));
}

void test_utf8_encode_buffer_too_small(void) {
    uint8_t buffer[2];
    size_t written;
    TEST_ASSERT_EQUAL(UTF8_ERR_BUFFER_TOO_SMALL, utf8_codepoint_encode(0x0800, buffer, sizeof(buffer), &written));
    TEST_ASSERT_EQUAL(3, written);
}

/*********************************
 * utf8_codepoint_decode tests
 *********************************/
void test_utf8_decode_valid_ascii(void) {
    const uint8_t input[] = {0x41};
    uint32_t codepoint;
    size_t consumed;
    TEST_ASSERT_EQUAL(UTF8_OK, utf8_codepoint_decode(input, sizeof(input), &codepoint, &consumed));
    TEST_ASSERT_EQUAL(0x41, codepoint);
    TEST_ASSERT_EQUAL(1, consumed);
}

void test_utf8_decode_valid_4byte(void) {
    const uint8_t input[] = {0xF0, 0x9F, 0x98, 0x8A};
    uint32_t codepoint;
    size_t consumed;
    TEST_ASSERT_EQUAL(UTF8_OK, utf8_codepoint_decode(input, sizeof(input), &codepoint, &consumed));
    TEST_ASSERT_EQUAL(0x1F60A, codepoint);
    TEST_ASSERT_EQUAL(4, consumed);
}

void test_utf8_decode_invalid_sequence(void) {
    const uint8_t input[] = {0xC3}; // Incomplete sequence
    uint32_t codepoint;
    size_t consumed;
    TEST_ASSERT_EQUAL(UTF8_ERR_INVALID_SEQUENCE, utf8_codepoint_decode(input, sizeof(input), &codepoint, &consumed));
}

void test_utf8_decode_overlong_encoding(void) {
    const uint8_t input[] = {0xC0, 0xAF}; // Overlong '/'
    uint32_t codepoint;
    size_t consumed;
    TEST_ASSERT_EQUAL(UTF8_ERR_INVALID_SEQUENCE, utf8_codepoint_decode(input, sizeof(input), &codepoint, &consumed));
}

void test_utf8_decode_surrogate_pair(void) {
    const uint8_t input[] = {0xED, 0xA0, 0x80}; // Invalid surrogate
    uint32_t codepoint;
    size_t consumed;
    TEST_ASSERT_EQUAL(UTF8_ERR_INVALID_SEQUENCE, utf8_codepoint_decode(input, sizeof(input), &codepoint, &consumed));
}

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
