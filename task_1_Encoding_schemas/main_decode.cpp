#include "decoder_service.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: base64-decode <input> [output]\n";
        return 1;
    }

    Config cfg;
    cfg.input_path = argv[1];

    if (argc >= 3) {
        cfg.output_path = argv[2];
    } else {
        std::string in_str = cfg.input_path.string();
        if (in_str.size() > 7 && in_str.substr(in_str.size() - 7) == ".base64") {
            cfg.output_path = in_str.substr(0, in_str.size() - 7);
        } else {
            cfg.output_path = in_str + ".decoded";
        }
    }

    auto res = DecoderService::run(cfg);

    if (!res.is_ok()) {
        std::cerr << res.error().to_string() << "\n";
        std::filesystem::remove(cfg.output_path); 
        return 2;
    }

    std::cout << "Decoded: " << cfg.output_path.string() << "\n";
    return 0;
}
