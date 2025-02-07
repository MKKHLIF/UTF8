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
    printf("UTF-8 Library Demo\n");
    printf("=================\n\n");

    // Example 1: Validate a UTF-8 string
    printf("1. String Validation\n");
    // "Hello, 世界!" in UTF-8
    const uint8_t hello[] = {
        0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, // "Hello, "
        0xE4, 0xB8, 0x96, 0xE7, 0x95, 0x8C, 0x21 // "世界!"
    };

    utf8_error_t err = utf8_str_validate(hello, sizeof(hello));
    printf("Validating \"Hello, 世界!\": %s\n", get_error_string(err));

    // Example 2: Count codepoints
    printf("\n2. Codepoint Counting\n");
    size_t count;
    err = utf8_str_count_codepoints(hello, sizeof(hello), &count);
    if (err == UTF8_OK) {
        printf("Number of codepoints: %zu\n", count);
    }

    // Example 3: Encode a codepoint
    printf("\n3. Codepoint Encoding\n");
    uint8_t buffer[4];
    size_t bytes_written;

    // Encode '世' (U+4E16)
    const uint32_t codepoint = 0x4E16;
    err = utf8_encode_cp(codepoint, buffer, sizeof(buffer), &bytes_written);
    if (err == UTF8_OK) {
        printf("Encoded U+4E16 ('世'): ");
        print_bytes(buffer, bytes_written);
    }

    // Example 4: Decode a sequence
    printf("\n4. Sequence Decoding\n");
    uint32_t decoded_codepoint;
    size_t bytes_consumed;

    // Decode the first character from our hello string
    err = utf8_decode_cp(hello, sizeof(hello), &decoded_codepoint, &bytes_consumed);
    if (err == UTF8_OK) {
        printf("Decoded first codepoint: U+%04X (consumed %zu bytes)\n",
               decoded_codepoint, bytes_consumed);
    }

    // Example 5: Error handling
    printf("\n5. Error Handling\n");

    // Try to encode an invalid codepoint
    err = utf8_encode_cp(0x110000, buffer, sizeof(buffer), &bytes_written);
    printf("Attempting to encode invalid codepoint (0x110000): %s\n",
           get_error_string(err));

    // Try to validate an invalid sequence
    const uint8_t invalid[] = {0xFF, 0xFF}; // Invalid UTF-8 sequence
    err = utf8_str_validate(invalid, sizeof(invalid));
    printf("Validating invalid sequence: %s\n", get_error_string(err));

    return 0;
}
