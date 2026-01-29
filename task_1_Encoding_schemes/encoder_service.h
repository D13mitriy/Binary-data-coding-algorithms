#pragma once
#include "common_types.h"
#include "base64_strategy.h"
#include <fstream>
#include <vector>

class EncoderService {
public:
    static Result<> run(const Config& cfg) {
        std::ifstream in(cfg.input_path, std::ios::binary);
        if (!in) return Result<>::err({0, 0, "Error opening input file"});

        std::ofstream out(cfg.output_path);
        if (!out) return Result<>::err({0, 0, "Error creating output file"});

        return process_stream(in, out, cfg.line_length);
    }

private:
    static Result<> process_stream(std::istream& in, std::ostream& out, size_t line_max) {
        uint8_t in_buf[3];
        char out_buf[4];
        size_t line_len = 0;

        while (in.read(reinterpret_cast<char*>(in_buf), 3) || in.gcount() > 0) {
            size_t bytes_read = in.gcount();
            
            Base64Strategy::encode_block(in_buf, bytes_read, out_buf);

            for (char c : out_buf) {
                out.put(c);
                if (++line_len >= line_max) {
                    out.put('\n');
                    line_len = 0;
                }
            }
            
            if (in.peek() == EOF) break; 
        }

        if (line_len > 0) out.put('\n');
        
        return Result<>::ok();
    }
};
