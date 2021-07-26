#include "catch.hpp"

#include "byte.hpp"

TEST_CASE("test ascii helpers") {
    using namespace hexview;

    REQUIRE(get_byte_category(0x00) == ByteCategory::AsciiOther);
    REQUIRE(get_byte_category(0x7F) == ByteCategory::AsciiOther);
    REQUIRE(get_byte_category(0x30) == ByteCategory::AsciiPrintable);
    REQUIRE(get_byte_category(0x0C) == ByteCategory::AsciiWhitespace);
}
