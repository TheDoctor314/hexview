#include "catch.hpp"

#include "byte.hpp"

TEST_CASE("test ascii helpers") {
    using namespace hexview;

    REQUIRE(get_byte_category(0x00) == ByteCategory::AsciiOther);
    REQUIRE(get_byte_category(0x7F) == ByteCategory::AsciiOther);
    REQUIRE(get_byte_category(0x30) == ByteCategory::AsciiPrintable);
    REQUIRE(get_byte_category(0x0C) == ByteCategory::AsciiWhitespace);
}

TEST_CASE("test to_int()") {
    using namespace hexview;
    using namespace std::string_view_literals;

#define SUCCESS(actual, expect) REQUIRE(to_int((actual)) == (expect))
#define ERROR(expr)             REQUIRE(to_int((expr)) == std::nullopt)

    SUCCESS("0", 0);
    SUCCESS("10", 10);
    SUCCESS("-10", -10);
    SUCCESS("+10", 10);

    SUCCESS("0x0", 0x0);
    SUCCESS("0xf", 0xf);
    SUCCESS("0x1234", 0x1234);
    SUCCESS("+0x1234", 0x1234);

    ERROR("");
    ERROR("+");
    ERROR("-");
    ERROR("0x-12");
    ERROR("0x+12");
    ERROR("1234abcd");

#undef SUCCESS
#undef ERROR
}
