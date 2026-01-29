#include "encoder_service.h"
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
    Config cfg;
    int arg_idx = 1;

    if (argc > 1) {
        if (std::strcmp(argv[1], "-s") == 0) {
            cfg.line_length = 64;
            arg_idx++;
        } else if (std::strcmp(argv[1], "-l") == 0) {
            cfg.line_length = 76;
            arg_idx++;
        }
    }

    if (arg_idx >= argc) {
        std::cerr << "Usage: base64-encode [-s|-l] <input> [output]\n";
        return 1;
    }

    cfg.input_path = argv[arg_idx++];
    cfg.output_path = (arg_idx < argc) ? argv[arg_idx] : cfg.input_path.string() + ".base64";

    auto res = EncoderService::run(cfg);

    if (!res.is_ok()) {
        std::cerr << "Encoder: " << res.error().to_string() << "\n";
        return 2;
    }
    
    std::cout << "Encoded: " << cfg.output_path.string() << "\n";
    return 0;
}
