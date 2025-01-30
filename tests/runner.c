#include "unity.h"
#include "utf8.h"
#include <stdint.h>
#include <stdbool.h>

void setUp(void) {
}

void tearDown(void) {
}

// Test cases for utf8_char_length
void test_utf8_char_length_ascii(void) {
    // Test ASCII character (1-byte sequence)
    TEST_ASSERT_EQUAL(1, utf8_char_length(0x41)); // 'A'
}

void test_utf8_char_length_two_byte(void) {
    // Test 2-byte UTF-8 sequence
    TEST_ASSERT_EQUAL(2, utf8_char_length(0xC3)); // First byte of 'é'
}

void test_utf8_char_length_invalid(void) {
    // Test invalid UTF-8 sequence
    TEST_ASSERT_EQUAL(0, utf8_char_length(0xFF)); // Invalid first byte
}

// Test cases for utf8_validate
void test_utf8_validate_valid(void) {
    // Test valid UTF-8 string
    uint8_t str[] = {0xC3, 0xA9, 0x6E, 0x64}; // "énd"
    TEST_ASSERT_TRUE(utf8_validate(str, sizeof(str)));
}

void test_utf8_validate_invalid(void) {
    // Test invalid UTF-8 string (incomplete sequence)
    uint8_t str[] = {0xC3}; // Incomplete 2-byte sequence
    TEST_ASSERT_FALSE(utf8_validate(str, sizeof(str)));
}

// Test cases for utf8_count_codepoints
void test_utf8_count_codepoints_valid(void) {
    // Test valid UTF-8 string
    uint8_t str[] = {0xC3, 0xA9, 0x6E, 0x64}; // "énd"
    TEST_ASSERT_EQUAL(3, utf8_count_codepoints(str, sizeof(str)));
}

void test_utf8_count_codepoints_invalid(void) {
    // Test invalid UTF-8 string
    uint8_t str[] = {0xC3}; // Incomplete 2-byte sequence
    TEST_ASSERT_EQUAL((size_t)-1, utf8_count_codepoints(str, sizeof(str)));
}

// Test cases for utf8_encode_codepoint
void test_utf8_encode_codepoint_ascii(void) {
    // Test encoding ASCII character
    uint8_t buffer[4] = {0};
    TEST_ASSERT_EQUAL(1, utf8_encode_codepoint(0x41, buffer, sizeof(buffer))); // 'A'
    TEST_ASSERT_EQUAL(0x41, buffer[0]);
}

void test_utf8_encode_codepoint_two_byte(void) {
    // Test encoding 2-byte UTF-8 sequence
    uint8_t buffer[4] = {0};
    TEST_ASSERT_EQUAL(2, utf8_encode_codepoint(0x00E9, buffer, sizeof(buffer))); // 'é'
    TEST_ASSERT_EQUAL(0xC3, buffer[0]);
    TEST_ASSERT_EQUAL(0xA9, buffer[1]);
}

void test_utf8_encode_codepoint_invalid(void) {
    // Test encoding invalid code point
    uint8_t buffer[4] = {0};
    TEST_ASSERT_EQUAL(0, utf8_encode_codepoint(0x110000, buffer, sizeof(buffer))); // Invalid code point
}

// Test cases for utf8_decode_codepoint
void test_utf8_decode_codepoint_ascii(void) {
    // Test decoding ASCII character
    uint8_t str[] = {0x41}; // 'A'
    TEST_ASSERT_EQUAL(0x41, utf8_decode_codepoint(str, sizeof(str)));
}

void test_utf8_decode_codepoint_two_byte(void) {
    // Test decoding 2-byte UTF-8 sequence
    uint8_t str[] = {0xC3, 0xA9}; // 'é'
    TEST_ASSERT_EQUAL(0x00E9, utf8_decode_codepoint(str, sizeof(str)));
}

void test_utf8_decode_codepoint_invalid(void) {
    // Test decoding invalid UTF-8 sequence
    uint8_t str[] = {0xC3}; // Incomplete 2-byte sequence
    TEST_ASSERT_EQUAL((uint32_t)-1, utf8_decode_codepoint(str, sizeof(str)));
}

// Test cases for utf8_iterate
void callback(uint32_t codepoint, void *context) {
    // Callback function for utf8_iterate
    (*(uint32_t *) context)++;
}

void test_utf8_iterate_valid(void) {
    // Test iterating over a valid UTF-8 string
    uint8_t str[] = {0xC3, 0xA9, 0x6E, 0x64}; // "énd"
    uint32_t count = 0;
    TEST_ASSERT_TRUE(utf8_iterate(str, sizeof(str), callback, &count));
    TEST_ASSERT_EQUAL(3, count); // 3 code points
}

void test_utf8_iterate_invalid(void) {
    // Test iterating over an invalid UTF-8 string
    uint8_t str[] = {0xC3}; // Incomplete 2-byte sequence
    uint32_t count = 0;
    TEST_ASSERT_FALSE(utf8_iterate(str, sizeof(str), callback, &count));
}

// Main function to run all tests
int main(void) {
    UNITY_BEGIN();

    // Run tests for utf8_char_length
    RUN_TEST(test_utf8_char_length_ascii);
    RUN_TEST(test_utf8_char_length_two_byte);
    RUN_TEST(test_utf8_char_length_invalid);

    // Run tests for utf8_validate
    RUN_TEST(test_utf8_validate_valid);
    RUN_TEST(test_utf8_validate_invalid);

    // Run tests for utf8_count_codepoints
    RUN_TEST(test_utf8_count_codepoints_valid);
    RUN_TEST(test_utf8_count_codepoints_invalid);

    // Run tests for utf8_encode_codepoint
    RUN_TEST(test_utf8_encode_codepoint_ascii);
    RUN_TEST(test_utf8_encode_codepoint_two_byte);
    RUN_TEST(test_utf8_encode_codepoint_invalid);

    // Run tests for utf8_decode_codepoint
    RUN_TEST(test_utf8_decode_codepoint_ascii);
    RUN_TEST(test_utf8_decode_codepoint_two_byte);
    RUN_TEST(test_utf8_decode_codepoint_invalid);

    // Run tests for utf8_iterate
    RUN_TEST(test_utf8_iterate_valid);
    RUN_TEST(test_utf8_iterate_invalid);

    return UNITY_END();
}
