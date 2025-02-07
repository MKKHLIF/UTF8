#include "utf8.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static bool is_continuation_byte(const uint8_t byte) {
    return (byte & 0xC0) == 0x80;
}

static size_t count_leading_ones(uint8_t byte) {
    size_t count = 0;
    while ((byte & 0x80) != 0) {
        count++;
        byte <<= 1;
    }
    return count;
}

static utf8_error_t validate_char(
    const uint8_t *input,
    const size_t input_len,
    size_t *bytes_consumed
) {
    if (input == NULL || bytes_consumed == NULL) {
        return UTF8_ERR_NULL_POINTER;
    }

    size_t char_len = count_leading_ones(input[0]);
    if (char_len == 1 || char_len > 4) {
        return UTF8_ERR_INVALID_SEQUENCE;
    }
    if (char_len == 0) {
        char_len = 1; // ASCII
    }

    if (input_len < char_len) {
        return UTF8_ERR_INVALID_SEQUENCE;
    }

    for (size_t i = 1; i < char_len; i++) {
        if (!is_continuation_byte(input[i])) {
            return UTF8_ERR_INVALID_SEQUENCE;
        }
    }

    *bytes_consumed = char_len;
    return UTF8_OK;
}

utf8_error_t utf8_str_validate(const uint8_t *input, const size_t input_len) {
    if (input == NULL) {
        return UTF8_ERR_NULL_POINTER;
    }

    size_t pos = 0;
    while (pos < input_len) {
        size_t bytes_consumed;
        utf8_error_t err = validate_char(&input[pos], input_len - pos, &bytes_consumed);
        if (err != UTF8_OK) {
            return err;
        }
        pos += bytes_consumed;
    }
    return UTF8_OK;
}

utf8_error_t utf8_str_count_codepoints(
    const uint8_t *input,
    size_t input_len,
    size_t *count_out
) {
    if (input == NULL || count_out == NULL) {
        return UTF8_ERR_NULL_POINTER;
    }

    size_t count = 0;
    size_t pos = 0;
    while (pos < input_len) {
        size_t bytes_consumed;
        utf8_error_t err = validate_char(&input[pos], input_len - pos, &bytes_consumed);
        if (err != UTF8_OK) {
            return err;
        }
        count++;
        pos += bytes_consumed;
    }
    *count_out = count;
    return UTF8_OK;
}

// ---------------------------------------- Public Interfaces ----------------------------------------

utf8_error_t utf8_codepoint_encode(
    const uint32_t codepoint,
    uint8_t *buffer_out,
    const size_t buffer_size,
    size_t *bytes_written_out
) {
    if (buffer_out == NULL || bytes_written_out == NULL) {
        return UTF8_ERR_NULL_POINTER;
    }

    // Validate codepoint range
    if (codepoint > 0x10FFFF || (codepoint >= 0xD800 && codepoint <= 0xDFFF)) {
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
        *bytes_written_out = bytes_needed;
        return UTF8_ERR_BUFFER_TOO_SMALL;
    }

    // Encode the codepoint
    switch (bytes_needed) {
        case 1:
            buffer_out[0] = (uint8_t) codepoint;
            break;
        case 2:
            buffer_out[0] = 0xC0 | ((codepoint >> 6) & 0x1F);
            buffer_out[1] = 0x80 | (codepoint & 0x3F);
            break;
        case 3:
            buffer_out[0] = 0xE0 | ((codepoint >> 12) & 0x0F);
            buffer_out[1] = 0x80 | ((codepoint >> 6) & 0x3F);
            buffer_out[2] = 0x80 | (codepoint & 0x3F);
            break;
        case 4:
            buffer_out[0] = 0xF0 | ((codepoint >> 18) & 0x07);
            buffer_out[1] = 0x80 | ((codepoint >> 12) & 0x3F);
            buffer_out[2] = 0x80 | ((codepoint >> 6) & 0x3F);
            buffer_out[3] = 0x80 | (codepoint & 0x3F);
            break;
        default: ;
    }

    *bytes_written_out = bytes_needed;
    return UTF8_OK;
}

utf8_error_t utf8_codepoint_decode(
    const uint8_t *input,
    const size_t input_len,
    uint32_t *codepoint_out,
    size_t *bytes_consumed_out
) {
    if (input == NULL || codepoint_out == NULL || bytes_consumed_out == NULL) {
        return UTF8_ERR_NULL_POINTER;
    }

    size_t char_len = count_leading_ones(input[0]);
    if (char_len == 0) {
        char_len = 1; // ASCII
    } else if (char_len == 1 || char_len > 4) {
        return UTF8_ERR_INVALID_SEQUENCE; // Invalid start byte
    }

    if (input_len < char_len) {
        return UTF8_ERR_INVALID_SEQUENCE; // Incomplete sequence
    }

    // Decode the codepoint
    uint32_t codepoint = 0;
    if (char_len == 1) {
        codepoint = input[0];
    } else {
        codepoint = input[0] & (0xFF >> (char_len + 1));
        for (size_t i = 1; i < char_len; i++) {
            if (!is_continuation_byte(input[i])) {
                return UTF8_ERR_INVALID_SEQUENCE;
            }
            codepoint = (codepoint << 6) | (input[i] & 0x3F);
        }
    }

    // Check for overlong encoding
    size_t expected_len;
    if (codepoint <= 0x7F) {
        expected_len = 1;
    } else if (codepoint <= 0x7FF) {
        expected_len = 2;
    } else if (codepoint <= 0xFFFF) {
        expected_len = 3;
    } else {
        expected_len = 4;
    }
    if (char_len != expected_len) {
        return UTF8_ERR_INVALID_SEQUENCE;
    }

    // Validate codepoint range
    if (codepoint > 0x10FFFF || (codepoint >= 0xD800 && codepoint <= 0xDFFF)) {
        return UTF8_ERR_INVALID_SEQUENCE;
    }
    *codepoint_out = codepoint;
    *bytes_consumed_out = char_len;
    return UTF8_OK;
}
