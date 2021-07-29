#pragma once

#include <ostream>

namespace hexview {
struct Colour {
    enum Code {
        None = 0,

        White,
        Red,
        Green,
        Yellow,
        Blue,
        Cyan,
    };

    Colour(std::ostream &out, const Code colour_code)
        : stream(out) {
        use(colour_code);
    }
    ~Colour() { use(None); }

  private:
    void use(const Code colour_code) {
        const auto &format = [](const Code code) {
            switch (code) {
            case None:
            case White:
            default:
                return "[0m";
            case Red:
                return "[0;31m";
            case Green:
                return "[0;32m";
            case Yellow:
                return "[0;33m";
            case Blue:
                return "[0;34m";
            case Cyan:
                return "[0;36m";
            }
        };

        stream << '\x1b' << format(colour_code);
    }

    std::ostream &stream;
};

} // namespace hexview
