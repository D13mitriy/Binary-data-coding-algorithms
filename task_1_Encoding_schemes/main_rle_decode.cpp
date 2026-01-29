#include "rle_service.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: rle-decode <input> [output]\n";
        return 1;
    }
    Config cfg;
    cfg.input_path = argv[1];
    if (argc >= 3) cfg.output_path = argv[2];
    else {
        std::string s = cfg.input_path.string();
        cfg.output_path = (s.ends_with(".rle")) ? s.substr(0, s.size()-4) : s + ".unrle";
    }

    auto res = RleService::decode(cfg);
    if (!res.is_ok()) {
        std::cerr << "RLE Decode Error: " << res.error().to_string() << "\n";
        return 2;
    }
    std::cout << "Decoded: " << cfg.output_path.string() << "\n";
    return 0;
}
