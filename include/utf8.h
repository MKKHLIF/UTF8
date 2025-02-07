/**
 * @file utf8.h
 * @brief UTF-8 Encoding/Decoding Library
 * @author MK
 * @date 2025-01-30
 * 
 * A minimal library for handling UTF-8 encoded strings in C.
 * Provides validation, encoding, decoding, and codepoint counting.
 */

#ifndef UTF8_H
#define UTF8_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Error codes returned by library functions.
 */
typedef enum {
    UTF8_OK, ///< Operation succeeded.
    UTF8_ERR_INVALID_CODEPOINT, ///< Invalid Unicode codepoint (e.g., > 0x10FFFF).
    UTF8_ERR_INVALID_SEQUENCE, ///< Invalid UTF-8 byte sequence.
    UTF8_ERR_BUFFER_TOO_SMALL, ///< Provided buffer is too small.
    UTF8_ERR_NULL_POINTER ///< NULL pointer passed where not allowed.
} utf8_error_t;

/**
 * @brief Encode a Unicode codepoint into a UTF-8 byte sequence.
 * 
 * @param codepoint Unicode codepoint (U+0000 to U+10FFFF).
 * @param buffer_out Output buffer to write the UTF-8 bytes.
 * @param buffer_size Size of `buffer_out` in bytes.
 * @param bytes_written_out Actual number of bytes written to `buffer_out`.
 * @return utf8_error_t 
 *   - UTF8_OK: Success.
 *   - UTF8_ERR_INVALID_CODEPOINT: Codepoint is invalid.
 *   - UTF8_ERR_BUFFER_TOO_SMALL: `buffer_out` is too small.
 *   - UTF8_ERR_NULL_POINTER: `buffer_out` or `bytes_written_out` is NULL.
 */
utf8_error_t utf8_codepoint_encode(
    uint32_t codepoint,
    uint8_t *buffer_out,
    size_t buffer_size,
    size_t *bytes_written_out
);

/**
 * @brief Decode a UTF-8 byte sequence into a Unicode codepoint.
 * 
 * @param input Pointer to the UTF-8 byte sequence.
 * @param input_len Length of the input in bytes.
 * @param codepoint_out Decoded Unicode codepoint.
 * @param bytes_consumed_out Actual number of bytes read from `input`.
 * @return utf8_error_t 
 *   - UTF8_OK: Success.
 *   - UTF8_ERR_INVALID_SEQUENCE: Invalid byte sequence.
 *   - UTF8_ERR_NULL_POINTER: `input`, `codepoint_out`, or `bytes_consumed_out` is NULL.
 */
utf8_error_t utf8_codepoint_decode(
    const uint8_t *input,
    size_t input_len,
    uint32_t *codepoint_out,
    size_t *bytes_consumed_out
);

#ifdef __cplusplus
}
#endif

#endif // UTF8_H
