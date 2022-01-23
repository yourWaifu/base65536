﻿#include <type_traits>
#include <string_view>
#include <string>
#include <cstring>
#include <array>
#include <unordered_map>
#include <charconv>
#include <iostream>
#include <locale>
#include <codecvt>

namespace Base65536 {

    constexpr uint32_t paddingBlockStart = 0x1500;
    constexpr std::array<uint32_t, 256> blockStarts = {
        0x03400, 0x03500, 0x03600, 0x03700, 0x03800, 0x03900, 0x03A00, 0x03B00, 0x03C00, 0x03D00,
        0x03E00, 0x03F00, 0x04000, 0x04100, 0x04200, 0x04300, 0x04400, 0x04500, 0x04600, 0x04700,
        0x04800, 0x04900, 0x04A00, 0x04B00, 0x04C00, 0x04E00, 0x04F00, 0x05000, 0x05100, 0x05200,
        0x05300, 0x05400, 0x05500, 0x05600, 0x05700, 0x05800, 0x05900, 0x05A00, 0x05B00, 0x05C00,
        0x05D00, 0x05E00, 0x05F00, 0x06000, 0x06100, 0x06200, 0x06300, 0x06400, 0x06500, 0x06600,
        0x06700, 0x06800, 0x06900, 0x06A00, 0x06B00, 0x06C00, 0x06D00, 0x06E00, 0x06F00, 0x07000,
        0x07100, 0x07200, 0x07300, 0x07400, 0x07500, 0x07600, 0x07700, 0x07800, 0x07900, 0x07A00,
        0x07B00, 0x07C00, 0x07D00, 0x07E00, 0x07F00, 0x08000, 0x08100, 0x08200, 0x08300, 0x08400,
        0x08500, 0x08600, 0x08700, 0x08800, 0x08900, 0x08A00, 0x08B00, 0x08C00, 0x08D00, 0x08E00,
        0x08F00, 0x09000, 0x09100, 0x09200, 0x09300, 0x09400, 0x09500, 0x09600, 0x09700, 0x09800,
        0x09900, 0x09A00, 0x09B00, 0x09C00, 0x09D00, 0x09E00, 0x0A100, 0x0A200, 0x0A300, 0x0A500,
        0x10600, 0x12000, 0x12100, 0x12200, 0x13000, 0x13100, 0x13200, 0x13300, 0x14400, 0x14500,
        0x16800, 0x16900, 0x20000, 0x20100, 0x20200, 0x20300, 0x20400, 0x20500, 0x20600, 0x20700,
        0x20800, 0x20900, 0x20A00, 0x20B00, 0x20C00, 0x20D00, 0x20E00, 0x20F00, 0x21000, 0x21100,
        0x21200, 0x21300, 0x21400, 0x21500, 0x21600, 0x21700, 0x21800, 0x21900, 0x21A00, 0x21B00,
        0x21C00, 0x21D00, 0x21E00, 0x21F00, 0x22000, 0x22100, 0x22200, 0x22300, 0x22400, 0x22500,
        0x22600, 0x22700, 0x22800, 0x22900, 0x22A00, 0x22B00, 0x22C00, 0x22D00, 0x22E00, 0x22F00,
        0x23000, 0x23100, 0x23200, 0x23300, 0x23400, 0x23500, 0x23600, 0x23700, 0x23800, 0x23900,
        0x23A00, 0x23B00, 0x23C00, 0x23D00, 0x23E00, 0x23F00, 0x24000, 0x24100, 0x24200, 0x24300,
        0x24400, 0x24500, 0x24600, 0x24700, 0x24800, 0x24900, 0x24A00, 0x24B00, 0x24C00, 0x24D00,
        0x24E00, 0x24F00, 0x25000, 0x25100, 0x25200, 0x25300, 0x25400, 0x25500, 0x25600, 0x25700,
        0x25800, 0x25900, 0x25A00, 0x25B00, 0x25C00, 0x25D00, 0x25E00, 0x25F00, 0x26000, 0x26100,
        0x26200, 0x26300, 0x26400, 0x26500, 0x26600, 0x26700, 0x26800, 0x26900, 0x26A00, 0x26B00,
        0x26C00, 0x26D00, 0x26E00, 0x26F00, 0x27000, 0x27100, 0x27200, 0x27300, 0x27400, 0x27500,
        0x27600, 0x27700, 0x27800, 0x27900, 0x27A00, 0x27B00, 0x27C00, 0x27D00, 0x27E00, 0x27F00,
        0x28000, 0x28100, 0x28200, 0x28300, 0x28400, 0x28500
    };

    struct ByteFromBlockStartResult {
        uint8_t byte;
        bool error = true;
    };

#if ( 201402L <= __cplusplus || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L) )
    constexpr
#endif
        ByteFromBlockStartResult getByteFromBlockStart(const uint32_t blockStart) {
#define CASE_BLOCKSTART2(x) case blockStarts[x]: return ByteFromBlockStartResult{x, false}; break; case blockStarts[x+1]: return ByteFromBlockStartResult{x+1, false}; break;
#define CASE_BLOCKSTART4(x) CASE_BLOCKSTART2(x) CASE_BLOCKSTART2(x+2)
#define CASE_BLOCKSTART8(x) CASE_BLOCKSTART4(x) CASE_BLOCKSTART4(x+4)
#define CASE_BLOCKSTART16(x) CASE_BLOCKSTART8(x) CASE_BLOCKSTART8(x+8)
#define CASE_BLOCKSTART32(x) CASE_BLOCKSTART16(x) CASE_BLOCKSTART16(x+16)
#define CASE_BLOCKSTART64(x) CASE_BLOCKSTART32(x) CASE_BLOCKSTART32(x+32)
#define CASE_BLOCKSTART128(x) CASE_BLOCKSTART64(x) CASE_BLOCKSTART64(x+64)
#define CASE_BLOCKSTART256 CASE_BLOCKSTART128(0) CASE_BLOCKSTART128(128)
        switch (blockStart) {
            CASE_BLOCKSTART256
        }
        return ByteFromBlockStartResult{ 255, true };
    }

#if ( 201703L <= __cplusplus || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) )

    template <std::size_t Y>
    struct EncodingResult {
        using Array = std::array<char32_t, Y>;
        std::array<char32_t, Y> data = {};
        std::size_t length = 0;
        static constexpr std::size_t getMaxSize() { return Y; }
        inline constexpr operator std::u32string_view() const { return { data.data(), length }; }
    };

    template <class X, std::size_t Y>
    constexpr auto encodePtr(const X* arr, std::size_t length = Y) {
        auto input = std::string_view{ arr, length };
        std::array<char32_t, Y> target = {};
        std::size_t targetSize = 0;

        const bool needsPadding = input.length() % 2 == 1;

        std::array<uint8_t, 2> btyeChuck = { 0,0 };
        int32_t byteSize = -1;
        for (std::size_t i = 0; i < input.length(); i += 1) {
            const uint8_t btye = static_cast<const uint8_t>(input[i]);
            if (i == (input.length() - 1) && needsPadding) {
                byteSize = 1;
                btyeChuck[0] = btye;
                btyeChuck[1] = 0;
            }
            else if (i % 2 == 1) {
                byteSize = 2;
                btyeChuck[1] = btye;
            }
            else {
                btyeChuck[0] = btye;
                continue;
            }

            auto blockStart = byteSize == 1
                ? paddingBlockStart
                : blockStarts[btyeChuck[1]];

            auto codePoint = blockStart + static_cast<uint32_t>(btyeChuck[0]);
            target[targetSize] = static_cast<const char32_t>(codePoint);
            targetSize += 1;
        }

        return EncodingResult<Y>{ std::move(target), targetSize };
    }

    template <class X, std::size_t Y>
    constexpr inline auto encode(const X(&arr)[Y], std::size_t length = Y - 1) {
        return encodePtr<X, Y>(arr, length);
    }

    template <
        class Array,
        typename std::enable_if<
        std::is_same<
        Array,
        std::array<typename Array::value_type, sizeof(Array) / sizeof(char)>
        >::value, bool
        >::type = false
    >
        constexpr inline auto encode(const Array& arr, std::size_t length = (sizeof(arr) / sizeof(arr[0]))) {
        return encodePtr<typename Array::value_type, arr.max_size()>(arr.data, length);
    }

    template <std::size_t Y>
    struct DecodingResult {
        using Array = std::array<char, Y>;
        std::array<char, Y> data = {};
        std::size_t length = 0;
        bool success = false;
        static constexpr std::size_t getMaxSize() { return Y; }
        inline constexpr operator std::string_view() const { return { data.data(), length }; }
    };

    template<class DecodingResult>
    constexpr inline auto encode(const DecodingResult& input) {
        return encode<typename DecodingResult::Array>(input.data(), input.length);
    }

    template <class X, std::size_t Y>
    constexpr auto decodePtr(const X* arr, std::size_t length = Y) {
        bool fail = false;
        bool done = false;
        auto input = std::u32string_view{ arr, length };
        std::array<char, Y> target = {};
        std::size_t targetSize = 0;

        for (std::size_t i = 0; i < input.length(); i += 1) {
            const char32_t codepoint = input[i];
            std::array<uint8_t, 2> byteChuck = {};
            const uint32_t blockMask = (1 << 8) - 1;
            byteChuck[0] = codepoint & blockMask;
            const auto blockStart = codepoint - byteChuck[0];
            byteChuck[1] = blockStart >> 8;

            if (static_cast<uint32_t>(blockStart) == paddingBlockStart) {
                if (done) {
                    fail = true;
                    break;
                }
                else {
                    target[targetSize] = byteChuck[0];
                    targetSize += 1;
                    done = true;
                }
            }
            else {
                const auto result = getByteFromBlockStart(blockStart);
                if (result.error || done) {
                    fail = true;
                    break;
                }

                target[targetSize] = static_cast<char>(byteChuck[0]);
                targetSize += 1;
                target[targetSize] = static_cast<char>(result.byte);
                targetSize += 1;
            }
        }

        return DecodingResult<Y>{
            std::move(target),
                targetSize, !fail
        };
    }

    template <
        class Array,
        typename std::enable_if<
        std::is_same<
        Array,
        std::array<typename Array::value_type, sizeof(Array) / sizeof(char32_t)>
        >::value, bool
        >::type = false
    >
        constexpr inline auto decode(
            const Array& arr, std::size_t length = sizeof(arr) / sizeof(arr[0])
        ) {
        return decodePtr<typename Array::value_type, sizeof(arr) / sizeof(arr[0])>(arr.data(), length);
    }

    template <class X, std::size_t Y>
    constexpr inline auto decode(const X(&arr)[Y], std::size_t length = Y - 1) {
        return decodePtr<X, Y>(arr, length);
    }

    template <class EncodingResult>
    constexpr auto decode(const EncodingResult& encodedInput) {
        return decode<typename EncodingResult::Array>(encodedInput.data, encodedInput.length);
    }

#endif

    std::u32string encode(const std::string& input) {
        std::size_t targetSize = input.length();
        std::u32string target;
        target.reserve(targetSize);

        const bool needsPadding = input.length() % 2 == 1;

        std::array<uint8_t, 2> btyeChuck = { 0,0 };
        int32_t byteSize = -1;
        for (std::size_t i = 0; i < input.length(); i += 1) {
            const uint8_t btye = static_cast<const uint8_t>(input[i]);
            if (i == (input.length() - 1) && needsPadding) {
                byteSize = 1;
                btyeChuck[0] = btye;
                btyeChuck[1] = 0;
            }
            else if (i % 2 == 1) {
                byteSize = 2;
                btyeChuck[1] = btye;
            }
            else {
                btyeChuck[0] = btye;
                continue;
            }

            auto blockStart = byteSize == 1
                ? paddingBlockStart
                : blockStarts[btyeChuck[1]];

            auto codePoint = blockStart + static_cast<uint32_t>(btyeChuck[0]);
            target.push_back(static_cast<const char32_t>(codePoint));
        }

        return target;
    }

    std::string decode(const std::u32string& input, bool& fail) {
        fail = false;
        bool done = false;
        std::string target = {};
        target.reserve(input.length() * 2);

        std::size_t length = 0;
        for (std::size_t i = 0; i < input.length(); i += 1) {
            const char32_t codepoint = input[i];
            std::array<uint8_t, 2> byteChuck = {};
            const uint32_t blockMask = (1 << 8) - 1;
            byteChuck[0] = codepoint & blockMask;
            const auto blockStart = codepoint - byteChuck[0];
            byteChuck[1] = blockStart >> 8;

            if (static_cast<uint32_t>(blockStart) == paddingBlockStart) {
                if (done) {
                    fail = true;
                    break;
                }
                else {
                    target += byteChuck[0];
                    done = true;
                }
            }
            else {
                const auto result = getByteFromBlockStart(blockStart);
                if (result.error || done) {
                    fail = true;
                    break;
                }
                target += {
                    static_cast<char>(byteChuck[0]),
                        static_cast<char>(result.byte)
                };
            }
        }

        return target;
    }

}
