#include <unity.h>
#include "utf8.h"

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

