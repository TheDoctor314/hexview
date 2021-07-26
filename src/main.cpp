#include <fstream>
#include <iomanip>
#include <iostream>

#include "printer.hpp"

void print_help(std::ostream &out);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        print_help(std::cerr);
        return 1;
    }

    auto printer = hexview::Printer(std::cout);
    std::ifstream input{argv[1], std::ios::binary};
    if (!input) {
        std::cerr << "Error: Could not read file\n";
        return 0;
    }

    printer.print_all(input);

    return 0;
}

void print_help(std::ostream &out) {
    out << "Usage: ./hexview [options] <filename>\n\n"
        << "Options:\n"
        << std::left << std::setw(10) << "-h"
        << "Print this help message\n"
        << std::setw(10) << "-l <N>"
        << "Only read N bytes from the input\n"
        << std::setw(10) << "-s <N>"
        << "Skip the first N bytes of the input\n";
}
