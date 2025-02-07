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
    *consumed = 0;
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

// Stream encoding function: takes a stream of codepoints, encodes each one as UTF-8, and outputs the sequence.
utf8_error_t utf8_encode_stream(
    const uint32_t *codepoints, // Input codepoints
    size_t codepoints_count, // Number of codepoints in the stream
    uint8_t *buffer, // Output buffer
    size_t buffer_size, // Size of the output buffer
    size_t *written // Number of bytes written to the buffer
) {
    // Validate input pointers
    if (codepoints == NULL || buffer == NULL || written == NULL) {
        return UTF8_ERR_NULL_POINTER;
    }

    *written = 0; // Initialize output
    size_t total_written = 0;

    // Iterate over the codepoint stream
    for (size_t i = 0; i < codepoints_count; i++) {
        size_t bytes_written = 0;

        // Check if there's enough space in the buffer for the worst-case scenario (4 bytes)
        if (total_written + 4 > buffer_size) {
            *written = total_written; // Return how much was written before the error
            return UTF8_ERR_BUFFER_TOO_SMALL;
        }

        // Encode the current codepoint
        utf8_error_t err = utf8_encode_cp(
            codepoints[i], // Current codepoint
            buffer + total_written, // Write to the buffer at the current position
            buffer_size - total_written, // Remaining buffer space
            &bytes_written // Bytes written for this codepoint
        );

        // Handle encoding errors
        if (err != UTF8_OK) {
            *written = total_written; // Return how much was written before the error
            return err;
        }

        // Update the total bytes written
        total_written += bytes_written;
    }

    // Success
    *written = total_written;
    return UTF8_OK;
}

utf8_error_t utf8_decode_stream(
    const uint8_t *buffer, // Input UTF-8 encoded buffer
    size_t buffer_size, // Size of the input buffer
    uint32_t *codepoints, // Output buffer for decoded codepoints
    size_t codepoints_size, // Size of the codepoints buffer
    size_t *decoded // Number of codepoints decoded
) {
    // Validate input pointers
    if (buffer == NULL || codepoints == NULL || decoded == NULL) {
        return UTF8_ERR_NULL_POINTER;
    }

    *decoded = 0; // Initialize output
    size_t total_decoded = 0;
    size_t consumed = 0;

    // Iterate over the input buffer
    while (consumed < buffer_size) {
        // Check if there's enough space in the codepoints buffer
        if (total_decoded >= codepoints_size) {
            *decoded = total_decoded; // Return how many codepoints were decoded before the error
            return UTF8_ERR_BUFFER_TOO_SMALL;
        }

        // Decode the next codepoint
        uint32_t cp;
        size_t bytes_consumed = 0;
        utf8_error_t err = utf8_decode_cp(
            buffer + consumed, // Current position in the buffer
            buffer_size - consumed, // Remaining buffer size
            &cp, // Decoded codepoint
            &bytes_consumed // Bytes consumed for this codepoint
        );

        // Handle decoding errors
        if (err != UTF8_OK) {
            *decoded = total_decoded; // Return how many codepoints were decoded before the error
            return err;
        }

        // Store the decoded codepoint
        codepoints[total_decoded] = cp;
        total_decoded++;
        consumed += bytes_consumed;
    }

    // Success
    *decoded = total_decoded;
    return UTF8_OK;
}
