#include <unity.h>
#include "utf8.h"

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
