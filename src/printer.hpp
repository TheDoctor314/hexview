#pragma once

#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>

#include "byte.hpp"

namespace hexview {
class Printer {
  public:
    Printer(std::ostream &out, u64 offset, bool colour)
        : m_display_offset(offset)
        , m_colour(colour)
        , m_writer(out) {}

    void set_length(u64 len);

    void print_position();
    void print_byte(u8 byte);
    void print_line();
    void print_all(std::istream &input);

  private:
    void print_header();
    void print_footer();

  private:
    u64 m_idx{1};
    std::optional<u64> m_remaining;
    u64 m_display_offset{0};
    bool m_colour;
    std::vector<u8> m_raw_line;
    std::stringstream m_output_line;
    std::ostream &m_writer;
};
} // namespace hexview
