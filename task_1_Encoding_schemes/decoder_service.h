#pragma once
#include "common_types.h"
#include "base64_strategy.h"
#include <fstream>
#include <string>
#include <cctype>

class DecoderService {
public:
    static Result<> run(const Config& cfg) {
        std::ifstream in(cfg.input_path);
        if (!in) return Result<>::err({0, 0, "Error opening input file"});

        std::ofstream out(cfg.output_path, std::ios::binary);
        if (!out) return Result<>::err({0, 0, "Error creating output file"});

        return process_stream(in, out);
    }

private:
    static Result<> process_stream(std::istream& in, std::ostream& out) {
        std::string line;
        size_t line_no = 0;
        size_t expected_len = 0;
        bool message_complete = false;

        while (std::getline(in, line)) {
            line_no++;

            // TRIM whitespace/CR from end of line (Fixes "Data after end" warnings on empty lines)
            while (!line.empty() && std::isspace(static_cast<unsigned char>(line.back()))) {
                line.pop_back();
            }

            if (line.empty() || line[0] == '|') continue;

            // STOP if we found data after the message was marked complete
            if (message_complete) {
                std::cerr << "Попередження: Наявні дані після кінця повідомлення\n";
                return Result<>::ok();
            }

            // --- VALIDATE LINE LENGTH ---
            bool is_last_line_candidate = false;

            if (line.length() % 4 != 0) {
                return Result<>::err({line_no, 0,
                                      std::format("Некоректна довжина рядку ({})", line.length())});
            }

            if (expected_len == 0) {
                // If it looks like a standard line (64 or 76), treat it as the standard
                if (line.length() == 64 || line.length() == 76) {
                    expected_len = line.length();
                } else {
                    // Otherwise, it must be a short last line
                    is_last_line_candidate = true;
                }
            } else if (line.length() != expected_len) {
                // Different length than established standard -> must be last line
                is_last_line_candidate = true;
            }

            if (is_last_line_candidate) {
                message_complete = true;
            }

            // --- PROCESS QUADS ---
            for (size_t i = 0; i < line.length(); i += 4) {
                char quad[4];
                for(int k=0; k<4; ++k) quad[k] = line[i+k];

                for (int k=0; k<4; ++k) {
                    if (quad[k] == '|') return Result<>::err({line_no, i+k+1, "Некоректний вхідний символ (|)"});

                    if (!Base64Strategy::is_valid_char(quad[k])) {
                        return Result<>::err({line_no, i+k+1,
                                              std::format("Некоректний вхідний символ (`{}`)", quad[k])});
                    }

                    if (quad[k] == '=') {
                        // Validate padding position
                        if (k < 2 && quad[2] != '=') return Result<>::err({line_no, i+k+1, "Неправильне використання паддінгу"});
                        if (k < 3 && quad[3] != '=') return Result<>::err({line_no, i+k+1, "Неправильне використання паддінгу"});

                        message_complete = true; // Padding always marks the end
                    }
                }

                uint8_t bytes[3];
                int valid_bytes = Base64Strategy::decode_block(quad, bytes);
                out.write(reinterpret_cast<char*>(bytes), valid_bytes);
            }
        }
        return Result<>::ok();
    }
};
