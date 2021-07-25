#pragma once

#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>

#include "byte.hpp"

namespace hexview {
class Printer {
  public:
    Printer(std::ostream &out)
        : m_writer(out) {}

    void print_position();
    void print_byte(u8 byte);
    void print_line();
    void print_all(std::istream &input);

  private:
    u64 m_idx{1};
    u64 m_display_offset{0};
    std::vector<u8> m_raw_line;
    std::stringstream m_output_line;
    std::ostream &m_writer;
};
} // namespace hexview
