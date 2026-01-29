#pragma once
#include <array>
#include <cstdint>
#include <string_view>
#include <algorithm>

// Define internals OUTSIDE the class to satisfy GCC 15 strictness
namespace Base64Internals {
constexpr std::string_view ALPHABET =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Use constexpr (not consteval) to allow more flexible evaluation ordering
constexpr std::array<int, 256> generate_table() {
    std::array<int, 256> table;
    table.fill(-1);
    for (size_t i = 0; i < ALPHABET.size(); ++i) {
        table[static_cast<unsigned char>(ALPHABET[i])] = static_cast<int>(i);
    }
    return table;
}
}

class Base64Strategy {
public:
    // Link to the external internals
    static constexpr std::string_view ALPHABET = Base64Internals::ALPHABET;
    static constexpr std::array<int, 256> DECODE_TABLE = Base64Internals::generate_table();

    static void encode_block(const uint8_t* in, size_t len, char* out) {
        uint32_t val = 0;
        for (size_t i = 0; i < len; ++i) val |= (in[i] << (16 - 8 * i));

        out[0] = ALPHABET[(val >> 18) & 0x3F];
        out[1] = ALPHABET[(val >> 12) & 0x3F];
        out[2] = (len > 1) ? ALPHABET[(val >> 6) & 0x3F] : '=';
        out[3] = (len > 2) ? ALPHABET[val & 0x3F] : '=';
    }

    static int decode_block(const char* in, uint8_t* out) {
        uint32_t val = 0;
        int padding = 0;

        for (int i = 0; i < 4; ++i) {
            if (in[i] == '=') {
                padding++;
                val <<= 6;
            } else {
                val = (val << 6) | DECODE_TABLE[static_cast<unsigned char>(in[i])];
            }
        }

        out[0] = (val >> 16) & 0xFF;
        if (padding < 2) out[1] = (val >> 8) & 0xFF;
        if (padding < 1) out[2] = val & 0xFF;

        return 3 - padding;
    }

    static bool is_valid_char(char c) {
        // Explicit cast to avoid signed char issues
        if (static_cast<unsigned char>(c) > 127) return false;
        return DECODE_TABLE[static_cast<unsigned char>(c)] != -1 || c == '=';
    }
};
