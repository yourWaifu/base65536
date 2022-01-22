#include "base65536.h"

int main() {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

    static constexpr char testInput[] =
        "Hello World";

    std::cout << testInput << '\n';
    constexpr auto test = encode(testInput);
    constexpr auto data = decode(test);
    std::cout << converter.to_bytes(std::u32string(test)) << '\n';
    std::cout << std::string_view{ data } << '\n';
}