#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

namespace hexview {
class Printer {
  public:
    Printer(std::ostream &out)
        : m_writer(out) {}

    void print_all(std::istream &input);

  private:
    uint64_t m_idx{1};
    std::vector<uint8_t> m_raw_line;
    std::vector<uint8_t> m_output_line;
    std::ostream &m_writer;
};
} // namespace hexview
