#include "base65536/base65536.h"

int main() {
    //std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    //IMPORTANT wstring_convert has been deprecated, we are only using for testing reasons

    using namespace Base65536;

    static constexpr char testInput[] =
        "hello world";

    std::cout << testInput << '\n';
    constexpr auto test = encode(testInput);
    constexpr auto data = decode(test);
    //std::cout << converter.to_bytes(std::u32string(test)) << '\n';
    std::cout << std::string_view(data) << '\n';
    std::cout << std::string(data).length() << " " << std::string(testInput).length() << '\n';
    static_assert(data.success);
    static_assert(std::string_view{ testInput }.compare(std::string_view{ data }) == 0);
    std::string runtimeTestInput = testInput;
    const auto runtimeTest = encode(runtimeTestInput);
    //std::cout << converter.to_bytes(runtimeTest) << '\n';
    int exitState = 0;
    if (runtimeTest == test.data.data()) {
        std::cout << "Encoder : output matches compile time version" << '\n';
    }
    else {
        std::cout << "Encoder : output doesn't match" << '\n';
        exitState = 1;
    }
    bool fail = false;
    const auto runtimeData = decode(runtimeTest, fail);
    std::cout << (fail ? "decode fail\n" : "decoded\n");
    if (runtimeData == testInput) {
        std::cout << "Decoder : output matches input" << '\n';
    }
    else {
        std::cout << "Decoder : output doesn't match" << '\n';
        exitState = 1;
    }
    return exitState;
}