#ifndef UTF8_H
#define UTF8_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


// Function to calculate the length (in bytes) of a UTF-8 encoded character
// given its first byte.
//
// Parameters:
//   first_byte: The first byte of the UTF-8 encoded character.
//
// Returns:
//   The length of the UTF-8 encoded character in bytes (1 to 4), or 0 if the
//   byte is not a valid start of a UTF-8 sequence.
size_t utf8_char_length(uint8_t first_byte);

// Function to validate a UTF-8 encoded string.
//
// Parameters:
//   str: A pointer to the UTF-8 encoded string.
//   length: The length of the string in bytes.
//
// Returns:
//   true if the string is valid UTF-8, false otherwise.
bool utf8_validate(const uint8_t *str, size_t length);

// Function to count the number of Unicode code points (characters) in a UTF-8
// encoded string.
//
// Parameters:
//   str: A pointer to the UTF-8 encoded string.
//   length: The length of the string in bytes.
//
// Returns:
//   The number of Unicode code points in the string, or (size_t)-1 if the
//   string is not valid UTF-8.
size_t utf8_count_codepoints(const uint8_t *str, size_t length);

// Function to encode a Unicode code point into a UTF-8 sequence.
//
// Parameters:
//   codepoint: The Unicode code point to encode.
//   buffer: A pointer to a buffer where the UTF-8 sequence will be stored.
//   buffer_size: The size of the buffer in bytes.
//
// Returns:
//   The number of bytes written to the buffer, or 0 if the code point is invalid
//   or the buffer is too small.
size_t utf8_encode_codepoint(uint32_t codepoint, uint8_t *buffer, size_t buffer_size);

// Function to decode a UTF-8 sequence into a Unicode code point.
//
// Parameters:
//   str: A pointer to the start of the UTF-8 sequence.
//   length: The length of the string in bytes.
//
// Returns:
//   The decoded Unicode code point, or (uint32_t)-1 if the sequence is invalid.
uint32_t utf8_decode_codepoint(const uint8_t *str, size_t length);

// Function to iterate over a UTF-8 encoded string and call a callback for each
// Unicode code point.
//
// Parameters:
//   str: A pointer to the UTF-8 encoded string.
//   length: The length of the string in bytes.
//   callback: A function pointer to the callback that will be called for each
//             code point. The callback receives the code point and a user-provided
//             context pointer.
//   context: A user-provided context pointer that will be passed to the callback.
//
// Returns:
//   true if the string was successfully iterated over, false if the string is
//   not valid UTF-8.
bool utf8_iterate(const uint8_t *str, size_t length, void (*callback)(uint32_t codepoint, void *context),
                  void *context);

#ifdef __cplusplus
}
#endif

#endif // UTF8_H
