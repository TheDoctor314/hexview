#pragma once

#include <charconv>
#include <cstdint>
#include <optional>
#include <string_view>

#include "colour.hpp"

namespace hexview {
using u8 = std::uint8_t;
using u64 = std::uint64_t;

enum class ByteCategory {
    AsciiPrintable,
    AsciiWhitespace,
    AsciiOther, // Control characters etc...
    NonAscii,
};

constexpr bool is_ascii_printable(u8 byte) {
    // between '!' and '~'
    return (byte >= 0x21) && (byte <= 0x7E);
}
constexpr bool is_ascii_whitespace(u8 byte) {
    return (byte == 0x20) || (byte == 0x09) || (byte == 0x0A) ||
           (byte == 0x0C) || (byte == 0x0D);
}
constexpr bool is_ascii(u8 byte) { return (byte & 128) == 0; }

constexpr auto get_byte_category(u8 byte) -> ByteCategory {
    if (is_ascii_printable(byte)) {
        return ByteCategory::AsciiPrintable;
    } else if (is_ascii_whitespace(byte)) {
        return ByteCategory::AsciiWhitespace;
    } else if (is_ascii(byte)) {
        return ByteCategory::AsciiOther;
    }

    return ByteCategory::NonAscii;
}

constexpr auto get_byte_colour(u8 byte) -> Colour::Code {
    switch (get_byte_category(byte)) {
    case ByteCategory::AsciiPrintable:
        return Colour::Cyan;
    case ByteCategory::AsciiWhitespace:
        return Colour::Green;
    case ByteCategory::AsciiOther:
        return Colour::White;
    case ByteCategory::NonAscii:
        return Colour::Yellow;
        break;
    }

    __builtin_unreachable();
}

constexpr auto as_char(u8 byte) -> char {
    switch (get_byte_category(byte)) {
    case ByteCategory::AsciiPrintable:
        return static_cast<char>(byte);
    case ByteCategory::AsciiWhitespace:
        if (byte == 0x20)
            return ' ';
        return '_';
    case ByteCategory::AsciiOther:
    case ByteCategory::NonAscii:
        return '.';
    }

    __builtin_unreachable();
}

inline std::optional<int> to_int(std::string_view input) {
    auto starts_with = [](auto &input, std::string_view prefix) -> bool {
        return input.substr(0, prefix.size()) == prefix;
    };

    static const auto HEX_PREFIX = std::string_view("0x");

    // from_chars() does not support parsing '+'
    if (input.front() == '+') {
        input.remove_prefix(1);
    }

    int base = 10;
    if (starts_with(input, HEX_PREFIX)) {
        input.remove_prefix(HEX_PREFIX.size());

        // there should be no sign after prefix
        if ((input.front() == '+') || (input.front() == '-')) {
            return {};
        }

        base = 16;
    }

    int out;
    auto [remaining_input, ec] =
        std::from_chars(input.data(), input.data() + input.size(), out, base);

    // error if all input is not consumed
    if (!std::string_view{remaining_input}.empty()) {
        return {};
    }

    // default std::errc indicates success
    if (ec == std::errc{}) {
        return out;
    }

    return {};
}
} // namespace hexview
