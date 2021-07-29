#include <fstream>
#include <iomanip>
#include <iostream>
#include <unistd.h>

#include "args.hpp"
#include "printer.hpp"

void print_help(std::ostream &out);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        print_help(std::cerr);
        return 1;
    }

    auto parser = hexview::ArgsParser{argc, argv};
    if (parser.option_exists("-h") || parser.option_exists("--help")) {
        print_help(std::cerr);
        return 1;
    }

    std::optional<int> length{};
    if (parser.option_exists("-l")) {
        auto len_or_err = parser.get_option("-l");
        if (len_or_err) {
            auto len = hexview::to_int(len_or_err.value());

            if (len) {
                std::cout << "length: " << len.value() << '\n';

                if (len.value() >= 0) {
                    length = len;
                }
            } else {
                std::cerr << "Error: Failed to parse '-l' argument\n";
                return 1;
            }
        } else {
            std::cerr << "Error: '-l' should have an argument\n";
            return 1;
        }
    }

    auto skip_offset = 0;
    if (parser.option_exists("-s")) {
        auto skip_or_err = parser.get_option("-s");
        if (skip_or_err) {
            auto len = hexview::to_int(skip_or_err.value());

            if (len) {
                std::cout << "skip: " << len.value() << '\n';
                skip_offset = len.value();
            } else {
                std::cerr << "Error: Failed to parse '-s' argument\n";
                return 1;
            }
        } else {
            std::cerr << "Error: '-s' should have an argument\n";
            return 1;
        }
    }

    bool show_colour = isatty(STDOUT_FILENO);
    if (parser.option_exists("--colour")) {
        auto colour_or_err = parser.get_option("--colour");
        if (colour_or_err) {
            const auto &colour = colour_or_err.value();
            if (colour == "always") {
                show_colour = true;
            } else if (colour == "never") {
                show_colour = false;
            } else if (colour == "auto") {
            } else {
                std::cerr << "Error: Failed to parse '--colour' argument\n";
                return 1;
            }
        } else {
            std::cerr << "Error: '--colour' should have an argument\n";
            return 1;
        }
    }

    std::ifstream input{argv[1], std::ios::binary};
    if (!input) {
        std::cerr << "Error: Could not read file: '" << argv[1] << "'\n";
        return 1;
    }

    if (skip_offset < 0) {
        input.seekg(skip_offset, std::ios_base::end);
        skip_offset = input.tellg();
    } else {
        input.seekg(skip_offset, std::ios_base::beg);
    }

    auto printer = hexview::Printer(std::cout, skip_offset, show_colour);

    if (length.has_value()) {
        printer.set_length(length.value());
    }
    printer.print_all(input);

    return 0;
}

void print_help(std::ostream &out) {
    out << "Usage: ./hexview <filename> [options]\n\n"
        << "Options:\n"
        << std::left << std::setw(20) << "-h"
        << "Print this help message\n"
        << std::setw(20) << "-l <N>"
        << "Only read N bytes from the input\n"
        << std::setw(20) << "-s <N>"
        << "Skip the first N bytes of the input\n"
        << std::setw(20) << "--colour <WHEN>"
        << "Control use of colour - always, auto, never\n";
}
