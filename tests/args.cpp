#include "catch.hpp"

#include <array>

#include "args.hpp"

TEST_CASE("test ArgsParser", "[args]") {
    using namespace hexview;

    constexpr std::array<const char *, 6> argv = {
        "prog_name", "-h", "-l", "10", "-s", "30",
    };

    auto parser = ArgsParser{argv.size(), argv.data()};

    REQUIRE(parser.option_exists("-h"));
    REQUIRE(parser.option_exists("-l"));
    REQUIRE(parser.option_exists("-s"));

    // just random options
    REQUIRE_FALSE(parser.option_exists("-a"));
    REQUIRE_FALSE(parser.option_exists("-f"));
    REQUIRE_FALSE(parser.option_exists("-o"));

    auto arg = parser.get_option("-l");
    REQUIRE((arg.has_value() && (arg.value() == "10")));

    arg = parser.get_option("-s");
    REQUIRE((arg.has_value() && (arg.value() == "30")));

    arg = parser.get_option("-a");
    REQUIRE_FALSE(arg.has_value());
}
