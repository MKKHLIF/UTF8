#include <stdio.h>
#include "utf8.h"

// Helper function to print byte sequences
void print_bytes(const uint8_t *bytes, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", bytes[i]);
    }
    printf("\n");
}

// Helper function to print errors
const char *get_error_string(utf8_error_t err) {
    switch (err) {
        case UTF8_OK: return "OK";
        case UTF8_ERR_INVALID_CODEPOINT: return "Invalid codepoint";
        case UTF8_ERR_INVALID_SEQUENCE: return "Invalid sequence";
        case UTF8_ERR_BUFFER_TOO_SMALL: return "Buffer too small";
        case UTF8_ERR_NULL_POINTER: return "Null pointer";
        default: return "Unknown error";
    }
}

int main() {
    uint32_t codepoints[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x1F600}; // "Hello" + 😀
    uint8_t buffer[32];
    size_t written;
    utf8_error_t err = utf8_encode_stream(codepoints, 6, buffer, sizeof(buffer), &written);
    if (err == UTF8_OK) {
        printf("Encoded %zu bytes :==> %s\n", written, buffer);
    } else {
        printf("Error: %d\n", err);
    }

    uint8_t buffer2[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0xF0, 0x9F, 0x98, 0x80}; // "Hello😀"
    uint32_t codepoints2[32];
    size_t decoded;
    utf8_error_t err2 = utf8_decode_stream(buffer2, sizeof(buffer2), codepoints2, 32, &decoded);
    if (err2 == UTF8_OK) {
        printf("Decoded %zu codepoints :==> ", decoded);
        for (size_t i = 0; i < decoded; i++) {
            printf("U+%04X ", codepoints[i]);
        }
    } else {
        printf("Error: %d\n", err2);
    }

    return 0;
}
