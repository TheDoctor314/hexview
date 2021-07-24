#include <algorithm>
#include <array>
#include <iomanip>

#include "printer.hpp"

namespace hexview {
enum class ByteCategory {
    Null,
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
    if (byte == 0x00) {
        return ByteCategory::Null;
    } else if (is_ascii_printable(byte)) {
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
    case ByteCategory::Null:
        return '0';
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

void Printer::print_all(std::istream &input) {
    std::array<char, 16> buffer{};

    for (;;) {
        input.read(buffer.data(), buffer.size());
        auto len = input.gcount();
        if (len == 0) {
            break;
        }

        for (auto i = 0; i < len; ++i) {
            print_byte(buffer[i]);
        }
    }

    print_line();
}

void Printer::print_byte(u8 byte) {
    if (m_idx % 16 == 1) {
        print_position();
    }
    m_raw_line.push_back(byte);

    {
        auto out_flags = m_output_line.flags();
        m_output_line << std::hex << std::setfill('0') << std::setw(2)
                      << static_cast<int>(byte);
        m_output_line.flags(out_flags);
    }

    if (m_idx % 16 == 0) {
        print_line();
    }

    m_idx++;
}

void Printer::print_position() {
    auto out_flags = m_output_line.flags();

    m_output_line << std::hex << std::setfill('0') << std::setw(8)
                  << (m_idx - 1) << ": ";

    m_output_line.flags(out_flags);
}

void Printer::print_line() {
    auto len = m_raw_line.size();

    m_output_line << "  ";
    if (len < 16) {
        auto out_flags = m_output_line.flags();
        m_output_line << std::setw((16 - len) * 2) << std::setfill(' ') << "";
        m_output_line.flags(out_flags);
    }

    for (auto byte : m_raw_line) {
        m_output_line << as_char(byte);
    }
    m_output_line << '\n';

    m_writer << m_output_line.str();

    m_raw_line.clear();
    m_output_line.str({});
}
} // namespace hexview
