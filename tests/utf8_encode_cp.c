#include <unity.h>
#include "utf8.h"

/*********************************
 * utf8_codepoint_encode tests
 *********************************/
void test_utf8_encode_valid_ascii(void) {
    uint8_t buffer[4];
    size_t written;
    TEST_ASSERT_EQUAL(UTF8_OK, utf8_encode_cp(0x41, buffer, sizeof(buffer), &written));
    TEST_ASSERT_EQUAL(1, written);
    TEST_ASSERT_EQUAL(0x41, buffer[0]);
}

void test_utf8_encode_valid_4byte(void) {
    uint8_t buffer[4];
    size_t written;
    TEST_ASSERT_EQUAL(UTF8_OK, utf8_encode_cp(0x1F60A, buffer, sizeof(buffer), &written));
    TEST_ASSERT_EQUAL(4, written);
    TEST_ASSERT_EQUAL(0xF0, buffer[0]);
    TEST_ASSERT_EQUAL(0x9F, buffer[1]);
    TEST_ASSERT_EQUAL(0x98, buffer[2]);
    TEST_ASSERT_EQUAL(0x8A, buffer[3]);
}

void test_utf8_encode_invalid_codepoint(void) {
    uint8_t buffer[4];
    size_t written;
    TEST_ASSERT_EQUAL(UTF8_ERR_INVALID_CODEPOINT, utf8_encode_cp(0x110000, buffer, sizeof(buffer), &written));
}

void test_utf8_encode_buffer_too_small(void) {
    uint8_t buffer[2];
    size_t written;
    TEST_ASSERT_EQUAL(UTF8_ERR_BUFFER_TOO_SMALL, utf8_encode_cp(0x0800, buffer, sizeof(buffer), &written));
    TEST_ASSERT_EQUAL(3, written);
}
