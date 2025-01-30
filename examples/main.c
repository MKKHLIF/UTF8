#include <stdint.h>
#include <stdio.h>

size_t utf8_encode_codepoint(const uint32_t codepoint, uint8_t *buffer, const size_t buffer_size) {
    // example:    0x1F4A9 =>  [0xF0, 0x9F, 0x92, 0xA9 ]


    // 7 bits or fewer - 1-byte sequence
    if (codepoint <= 0x7F) {
        if (buffer_size < 1) return 0;
        buffer[0] = (uint8_t) codepoint;
        return 1;
    }

    // 11 bits or fewer - 2-byte sequence
    if (codepoint <= 0x7FF) {
        if (buffer_size < 2) return 0;

        buffer[0] = (uint8_t) (0xC0 | ((codepoint >> 6) & 0x1F));
        buffer[1] = (uint8_t) (0x80 | (codepoint & 0x3F));
        return 2;
    }

    // 16 bits or fewer - 3-byte sequence
    if (codepoint <= 0xFFFF) {
        if (buffer_size < 3) return 0;

        // codepoint = 1111111111101011 / 0xFFEB
        // 0: 00001111 & 00001111 | 11100000 => 11101111       0xEF
        // 1: 1111111111 & 0000111111 | 10000000 => 10111111    0xBF
        // 2: 1111111111101011 & 0000000000111111 | 0000000010000000 => 10101011    0xAB

        buffer[0] = (uint8_t) (0xE0 | ((codepoint >> 12) & 0x0F));
        buffer[1] = (uint8_t) (0x80 | ((codepoint >> 6) & 0x3F));
        buffer[2] = (uint8_t) (0x80 | (codepoint & 0x3F));

        return 3;
    }

    // 21 bits or fewer - 4-byte sequence
    if (codepoint <= 0x10FFFF) {
        if (buffer_size < 4) return 0;
        // codepoint = 00011111010010101001 / 0x1F4A9
        // 0: 00000000 & 00000111 | 11110000 => 11110000    0xF0
        // 1: 00011111 & 00111111 | 10000000 => 10011111    0x9F
        // 2: 00011111010010 & 00000000111111 | 10000000 => 10010010    0x92
        // 3: 00011111010010101001 & 000000000000111111 | 10000000 => 10101001  0xA9
        buffer[0] = (uint8_t) (0xF0 | ((codepoint >> 18) & 0x07));
        buffer[1] = (uint8_t) (0x80 | ((codepoint >> 12) & 0x3F));
        buffer[2] = (uint8_t) (0x80 | ((codepoint >> 6) & 0x3F));
        buffer[3] = (uint8_t) (0x80 | (codepoint & 0x3F));
        return 4;
    }

    // Invalid codepoint
    return 0;
}

size_t utf8_char_length(uint8_t leading_byte) {
    size_t leading_ones = 0;

    while (leading_byte && (leading_byte & 0x80) != 0) {
        leading_byte <<= 1;
        ++leading_ones;
    }

    if (leading_ones == 0) return 1; // ASCII char
    if (leading_ones == 1) return 0; // not a leading byte
    if (leading_ones <= 4) return leading_ones;

    return 0;
}

uint32_t utf8_decode_codepoint(const uint8_t *buffer) {
    // Function to decode a UTF-8 sequence into a Unicode code point
    // example:     [0xF0, 0x9F, 0x92, 0xA9 ]               => 0x1F4A9
    //              [11110000 10011111 10010010 10101001]   => 00011111010010101001

    // 11110000 & 00000111 => 0
    // 00000000 | (10011111 & 00111111) => 00011111
    // 00011111000000 | 010010 => 00011111010010
    // 00011111010010000000 | 00101001 => 00011111010010101001 0x1F4A9

    const size_t char_len = utf8_char_length(buffer[0]);

    if (char_len == 0) {
        return -1; // Invalid sequence
    }

    uint32_t codepoint = 0;
    if (char_len == 1) {
        // ASCII char
        codepoint = buffer[0];
    } else {
        codepoint = buffer[0] & (0xFF >> (char_len + 1));
        for (size_t i = 1; i < char_len; i++) {
            if ((buffer[i] & 0xC0) != 0x80) {
                return -1; // Invalid continuation byte
            }
            codepoint = (codepoint << 6) | (buffer[i] & 0x3F);
        }
    }

    // Check for overlong encoding
    if (utf8_encode_codepoint(codepoint, (uint8_t[4]){0}, 4) != char_len) {
        return -1;
    }

    return codepoint;
}


void print_buffer(const uint8_t *buffer, const size_t buffer_size) {
    printf("[");
    for (size_t i = 0; i < buffer_size; i++) {
        printf(" 0x%02X ", buffer[i]);
    }
    printf("]");
}


int main() {
    const uint32_t codepoint = 0x1F4A9;
    const size_t buffer_size = 4;
    uint8_t buffer[buffer_size];

    const size_t written = utf8_encode_codepoint(codepoint, buffer, buffer_size);
    printf("%lu byte(s) written.\n", written);
    print_buffer(buffer, buffer_size);


    return 0;
}




