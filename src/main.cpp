#include <fstream>
#include <iostream>

#include "printer.hpp"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "Usage: hexview <filename>\n";
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
