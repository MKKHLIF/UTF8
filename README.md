# UTF-8 Encoding/Decoding Library

## Overview

This is a minimal library for handling UTF-8 encoded strings in C. It provides functions for validation, encoding, decoding, and codepoint counting.

## Features

- Encode a Unicode codepoint into a UTF-8 byte sequence.
- Decode a UTF-8 byte sequence into a Unicode codepoint.
- Encode a stream of Unicode codepoints into a UTF-8 byte sequence.
- Decode a UTF-8 byte sequence into a stream of Unicode codepoints.

## Usage

### Encoding a Unicode codepoint

```c
#include "utf8.h"

uint8_t buffer[4];
size_t written;
utf8_error_t result = utf8_encode_cp(0x1F600, buffer, sizeof(buffer), &written);
if (result == UTF8_OK) {
    // buffer contains the UTF-8 encoded bytes
}
```

### Encoding a Unicode codepoint

```c
#include "utf8.h"

const uint8_t sequence[] = {0xF0, 0x9F, 0x98, 0x80};
uint32_t codepoint;
size_t consumed;
utf8_error_t result = utf8_decode_cp(sequence, sizeof(sequence), &codepoint, &consumed);
if (result == UTF8_OK) {
    // codepoint contains the decoded Unicode codepoint
}
```
## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
