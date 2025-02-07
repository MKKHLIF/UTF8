#include "utf8.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


static bool is_continuation_byte(const uint8_t byte) {
    return (byte & 0xC0) == 0x80;
}

static bool is_valid_codepoint(const uint32_t cp) {
    if (cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF)) return false;
    return true;
}

static size_t count_leading_ones(uint8_t byte) {
    size_t count = 0;
    while ((byte & 0x80) != 0) {
        count++;
        byte <<= 1;
    }
    return count;
}

utf8_error_t utf8_encode_cp(
    const uint32_t codepoint,
    uint8_t *buffer,
    const size_t buffer_size,
    size_t *written
) {
    if (buffer == NULL || written == NULL) {
        return UTF8_ERR_NULL_POINTER;
    }

    // Validate codepoint range
    if (!is_valid_codepoint(codepoint)) {
        return UTF8_ERR_INVALID_CODEPOINT;
    }

    size_t bytes_needed;
    if (codepoint <= 0x7F) {
        bytes_needed = 1;
    } else if (codepoint <= 0x7FF) {
        bytes_needed = 2;
    } else if (codepoint <= 0xFFFF) {
        bytes_needed = 3;
    } else {
        bytes_needed = 4;
    }

    if (buffer_size < bytes_needed) {
        *written = bytes_needed;
        return UTF8_ERR_BUFFER_TOO_SMALL;
    }

    // Encode the codepoint
    switch (bytes_needed) {
        case 1:
            buffer[0] = (uint8_t) codepoint;
            break;
        case 2:
            buffer[0] = 0xC0 | ((codepoint >> 6) & 0x1F);
            buffer[1] = 0x80 | (codepoint & 0x3F);
            break;
        case 3:
            buffer[0] = 0xE0 | ((codepoint >> 12) & 0x0F);
            buffer[1] = 0x80 | ((codepoint >> 6) & 0x3F);
            buffer[2] = 0x80 | (codepoint & 0x3F);
            break;
        case 4:
            buffer[0] = 0xF0 | ((codepoint >> 18) & 0x07);
            buffer[1] = 0x80 | ((codepoint >> 12) & 0x3F);
            buffer[2] = 0x80 | ((codepoint >> 6) & 0x3F);
            buffer[3] = 0x80 | (codepoint & 0x3F);
            break;
        default: ;
    }

    *written = bytes_needed;
    return UTF8_OK;
}

utf8_error_t utf8_decode_cp(
    const uint8_t *sequence,
    const size_t sequence_size,
    uint32_t *codepoint,
    size_t *consumed
) {
    if (sequence == NULL || codepoint == NULL || consumed == NULL) {
        return UTF8_ERR_NULL_POINTER;
    }

    size_t char_len = count_leading_ones(sequence[0]);
    if (char_len == 0) {
        char_len = 1; // ASCII
    } else if (char_len == 1 || char_len > 4) {
        return UTF8_ERR_INVALID_SEQUENCE; // Invalid start byte
    }

    if (sequence_size < char_len) {
        return UTF8_ERR_INVALID_SEQUENCE; // Incomplete sequence
    }

    // Decode the codepoint
    uint32_t cp = 0;
    if (char_len == 1) {
        cp = sequence[0];
    } else {
        cp = sequence[0] & (0xFF >> (char_len + 1));
        for (size_t i = 1; i < char_len; i++) {
            if (!is_continuation_byte(sequence[i])) {
                return UTF8_ERR_INVALID_SEQUENCE;
            }
            cp = (cp << 6) | (sequence[i] & 0x3F);
        }
    }

    // Check for overlong encoding
    size_t expected_len;
    if (cp <= 0x7F) {
        expected_len = 1;
    } else if (cp <= 0x7FF) {
        expected_len = 2;
    } else if (cp <= 0xFFFF) {
        expected_len = 3;
    } else {
        expected_len = 4;
    }
    if (char_len != expected_len) {
        return UTF8_ERR_INVALID_SEQUENCE;
    }

    // Validate codepoint range
    if (!is_valid_codepoint(cp)) {
        return UTF8_ERR_INVALID_SEQUENCE;
    }
    *codepoint = cp;
    *consumed = char_len;
    return UTF8_OK;
}
