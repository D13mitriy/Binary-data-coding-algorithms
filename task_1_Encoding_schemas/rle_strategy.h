#pragma once
#include <cstdint>
#include <vector>

class RleStrategy {
public:
    static constexpr size_t MAX_RUN_LITERAL = 128; // 1..128
    static constexpr size_t MAX_RUN_REPEAT = 129;  // 2..129

    // Encoding Helpers
    static uint8_t make_repeat_header(size_t length) {
        // l bytes -> L = 128 + l - 2
        return static_cast<uint8_t>(128 + (length - 2));
    }

    static uint8_t make_literal_header(size_t length) {
        // l bytes -> L = l - 1
        return static_cast<uint8_t>(length - 1);
    }

    // Decoding Helpers
    static bool is_repeat_packet(uint8_t header) {
        return (header & 0x80) != 0;
    }

    static size_t get_repeat_length(uint8_t header) {
        return (header & 0x7F) + 2;
    }

    static size_t get_literal_length(uint8_t header) {
        return static_cast<size_t>(header) + 1;
    }
};
