# Utf8 Library

A compact C library implementing [RFC 3629](https://tools.ietf.org/html/rfc3629) for UTF-8 encoding and decoding of Unicode characters.

## Getting Started

This library encodes and decodes Unicode characters to ensure efficient storage and transmission. Ideal for integrating with applications that handle international text.

## Supported Platforms

- **Windows**: Visual Studio (MSVC)
- **Linux**: GCC or Clang
- **MacOS**: Xcode (Clang)

## Prerequisites

- [CMake](https://cmake.org/) 3.8 or later
- A C compiler compatible with CMake

## Building the Library

### Step 1: Generate Build System

Create a build directory and generate the build system using CMake:

```sh
mkdir build
cd build
cmake -G "Unix Makefiles" ..
```

### Step 2: Compile and Link

Navigate to the build directory and compile the project using CMake:

```sh
cd build
cmake --build .
```

## Usage Example

```c
#include "utf8.h"
#include <stdio.h>

int main() {
    // Example Unicode code points
    uint32_t codepoints[] = {0x0048, 0x0065, 0x006C, 0x006C, 0x006F, 0x002C, 0x0020, 0x4E16, 0x754C, 0x0021, 0}; // "Hello, 世界!"

    // Buffer to hold encoded UTF-8 string
    char utf8_str[256];
    
    // Encode Unicode code points to UTF-8
    int utf8_length = utf8_encode(codepoints, utf8_str, sizeof(utf8_str));

    if (utf8_length >= 0) {
        printf("Encoded UTF-8 string: %s\n", utf8_str);
        
        // Decode UTF-8 string back to Unicode code points
        uint32_t decoded_codepoints[256];
        int decoded_length = utf8_decode(utf8_str, decoded_codepoints, sizeof(decoded_codepoints) / sizeof(uint32_t));
        
        if (decoded_length >= 0) {
            printf("Decoded Unicode code points:\n");
            for (int i = 0; i < decoded_length; i++) {
                printf("U+%04X ", decoded_codepoints[i]);
            }
            printf("\n");
        } else {
            printf("Error decoding UTF-8 string.\n");
        }
    } else {
        printf("Error encoding Unicode code points.\n");
    }

    return 0;
}

```
## License
Licensed under the [MIT license](LICENSE.md).