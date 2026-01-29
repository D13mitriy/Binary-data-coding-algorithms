#include "rle_service.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: rle-encode <input> [output]\n";
        return 1;
    }
    Config cfg;
    cfg.input_path = argv[1];
    cfg.output_path = (argc >= 3) ? argv[2] : cfg.input_path.string() + ".rle";

    auto res = RleService::encode(cfg);
    if (!res.is_ok()) {
        std::cerr << "RLE Encode Error: " << res.error().to_string() << "\n";
        return 2;
    }
    std::cout << "Encoded: " << cfg.output_path.string() << "\n";
    return 0;
}
