#pragma once
#include "common_types.h"
#include "rle_strategy.h"
#include <fstream>
#include <vector>

class RleService {
public:
    // --- ENCODER ---
    static Result<> encode(const Config& cfg) {
        std::ifstream in(cfg.input_path, std::ios::binary);
        if (!in) return Result<>::err({0,0, "Error opening input file"});
        std::ofstream out(cfg.output_path, std::ios::binary);
        if (!out) return Result<>::err({0,0, "Error creating output file"});

        // Load file to memory for easy lookahead
        std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        if (data.empty()) return Result<>::ok();

        size_t i = 0;
        while (i < data.size()) {
            // 1. Try to find a run of identical bytes
            size_t run_len = 1;
            while ((i + run_len < data.size()) && 
                   (data[i + run_len] == data[i]) && 
                   (run_len < RleStrategy::MAX_RUN_REPEAT)) {
                run_len++;
            }

            // Case A: REPEAT RUN (>= 2 bytes)
            if (run_len >= 2) {
                uint8_t header = RleStrategy::make_repeat_header(run_len);
                out.put(static_cast<char>(header));
                out.put(static_cast<char>(data[i]));
                i += run_len;
            } 
            // Case B: LITERAL RUN
            else {
                size_t lit_len = 0;
                size_t j = i;
                
                // Accumulate literals until we hit max size OR start of a repeat run
                while (j < data.size() && lit_len < RleStrategy::MAX_RUN_LITERAL) {
                    if (j + 1 < data.size() && data[j] == data[j+1]) {
                        break; // Repeat detected, stop literals
                    }
                    lit_len++;
                    j++;
                }

                uint8_t header = RleStrategy::make_literal_header(lit_len);
                out.put(static_cast<char>(header));
                out.write(reinterpret_cast<const char*>(&data[i]), lit_len);
                i += lit_len;
            }
        }
        return Result<>::ok();
    }

    // --- DECODER ---
    static Result<> decode(const Config& cfg) {
        std::ifstream in(cfg.input_path, std::ios::binary);
        if (!in) return Result<>::err({0,0, "Error opening input file"});
        std::ofstream out(cfg.output_path, std::ios::binary);
        if (!out) return Result<>::err({0,0, "Error creating output file"});

        char header_char;
        while (in.get(header_char)) {
            uint8_t header = static_cast<uint8_t>(header_char);

            if (RleStrategy::is_repeat_packet(header)) {
                // REPEAT PACKET
                size_t count = RleStrategy::get_repeat_length(header);
                char val;
                if (!in.get(val)) {
                     return Result<>::err({0, 0, "Неочікуваний кінець файлу (Repeat Value)"});
                }
                for (size_t k = 0; k < count; ++k) out.put(val);
            } else {
                // LITERAL PACKET
                size_t count = RleStrategy::get_literal_length(header);
                std::vector<char> buf(count);
                in.read(buf.data(), count);
                if (in.gcount() != static_cast<std::streamsize>(count)) {
                    return Result<>::err({0, 0, "Неочікуваний кінець файлу (Literal Sequence)"});
                }
                out.write(buf.data(), count);
            }
        }
        return Result<>::ok();
    }
};
