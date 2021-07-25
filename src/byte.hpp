#pragma once

#include <cstdint>
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

} // namespace hexview
