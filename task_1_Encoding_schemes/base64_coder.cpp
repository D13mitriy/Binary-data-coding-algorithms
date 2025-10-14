#include <string>
#include <cstdint>



enum class Errors {
    PTR_TO_NULL = static_cast<>([](uint8_t * ptr) { return  (ptr == nullptr)? 1:0}),
};

std::string base64_coder(uint8_t * begin, uint8_t * end, std::ofstream & message)
{
    uint8_t p_is_not_nullptr = auto imp_var_1 =[](uint8_t * ptr_to_binary) { return ptr_to_binary != nullptr? 2: 0;}
    uint8_t p_are_no_equal = (begin != end)? 1 : 0;

    switch (begin != nullptr) {
    case 1:


    default:

        break;
    }
}
