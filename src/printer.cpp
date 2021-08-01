#include <algorithm>
#include <array>
#include <iomanip>

#include "colour.hpp"
#include "printer.hpp"

namespace hexview {
struct BorderElements {
    BorderElements(char left, char right, char column, char line)
        : left_corner(left)
        , right_corner(right)
        , column_sep(column)
        , horizontal_line(line) {}

    static auto border() -> BorderElements {
        return BorderElements('+', '+', '+', '-');
    }

    static auto separator() -> char { return '|'; }

    char left_corner;
    char right_corner;
    char column_sep;
    char horizontal_line;
};

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

    print_footer();
}

void Printer::print_byte(u8 byte) {
    if (m_idx % 16 == 1) {
        print_position();
    }
    m_raw_line.push_back(byte);

    {
        Colour guard{m_output_line,
                     m_colour ? get_byte_colour(byte) : Colour::None};
        auto out_flags = m_output_line.flags();
        m_output_line << std::hex << std::setfill('0') << std::setw(2)
                      << static_cast<int>(byte) << ' ';
        m_output_line.flags(out_flags);
    }

    if (m_idx % 16 == 0) {
        print_line();
    }

    m_idx++;
}

void Printer::print_position() {
    if (m_idx == 1) {
        print_header();
    }

    m_output_line << BorderElements::separator();
    auto out_flags = m_output_line.flags();

    m_output_line << std::hex << std::setfill('0') << std::setw(8)
                  << (m_idx - 1 + m_display_offset);

    m_output_line.flags(out_flags);
    m_output_line << BorderElements::separator();
}

void Printer::print_line() {
    auto len = m_raw_line.size();

    if (len < 16) {
        auto out_flags = m_output_line.flags();
        m_output_line << std::setw((16 - len) * 3) << std::setfill(' ') << "";
        m_output_line.flags(out_flags);
    }

    m_output_line << BorderElements::separator();

    for (auto byte : m_raw_line) {
        m_output_line << as_char(byte);
    }

    if (len < 16) {
        auto out_flags = m_output_line.flags();
        m_output_line << std::setw(16 - len) << std::setfill(' ') << "";
        m_output_line.flags(out_flags);
    }

    m_output_line << BorderElements::separator();
    m_output_line << '\n';

    m_writer << m_output_line.str();

    m_raw_line.clear();
    m_output_line.str({});
}

void Printer::print_header() {
    auto [l, r, c, line] = BorderElements::border();
    auto line8 = std::string(8, line);
    auto line48 = std::string(48, line);

    m_writer << l << line8 << c << line48 << c << line8 << line8 << r << '\n';
}
void Printer::print_footer() {
    auto [l, r, c, line] = BorderElements::border();
    auto line8 = std::string(8, line);
    auto line48 = std::string(48, line);

    m_writer << l << line8 << c << line48 << c << line8 << line8 << r << '\n';
}
} // namespace hexview
