#include <array>

#include "printer.hpp"

void hexview::Printer::print_all(std::istream &input) {
    (void)m_idx;

    std::array<char, 16> buffer{};

    input.read(buffer.data(), buffer.size());

    m_writer.write(buffer.data(), buffer.size());
}
