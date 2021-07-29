#include <algorithm>
#include <array>
#include <iomanip>

#include "colour.hpp"
#include "printer.hpp"

namespace hexview {
void Printer::set_length(u64 len) { m_remaining = len - m_idx + 1; }

void Printer::print_all(std::istream &input) {
    std::array<char, 16> buffer{};

    for (;;) {
        input.read(buffer.data(), buffer.size());
        std::size_t len = input.gcount();

        if (m_remaining) {
            if (len > m_remaining.value()) {
                len = m_remaining.value();
            }
            m_remaining = m_remaining.value() - len;
        }
        if (len == 0) {
            break;
        }

        for (std::size_t i = 0; i < len; ++i) {
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
        Colour guard{m_output_line, get_byte_colour(byte)};
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
                  << (m_idx - 1 + m_display_offset) << ": ";

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
