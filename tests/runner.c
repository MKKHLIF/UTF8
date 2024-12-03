#include "unity.h"

extern void test_md5(void);
extern void test_sha1(void);
extern void test_sha2(void);


void setUp(void) {
}

void tearDown(void) {
}

int main(void) {
    UNITY_BEGIN();

    return UNITY_END();
}