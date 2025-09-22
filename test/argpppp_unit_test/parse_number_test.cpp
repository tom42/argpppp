// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <concepts>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <type_traits>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::parse_integral;
using argpppp::parse_integral_result;

namespace
{

template <std::integral TValue>
struct testdata final
{
    testdata(std::string input, TValue expected_value, parse_integral_result expected_parse_result)
        : input(input)
        , expected_value(expected_value)
        , expected_parse_result(expected_parse_result)
    {}

    std::string input;
    TValue expected_value;
    parse_integral_result expected_parse_result;
};

}

TEST_CASE("parse_integral")
{
    SECTION("parse int64_t")
    {
        auto data = GENERATE(
            testdata("-9223372036854775809", int64_t(0x8000000000000000), parse_integral_result::underflow),
            testdata("-9223372036854775808", int64_t(0x8000000000000000), parse_integral_result::success),
            testdata(" 9223372036854775807", int64_t(0x7fffffffffffffff), parse_integral_result::success),
            testdata(" 9223372036854775808", int64_t(0x7fffffffffffffff), parse_integral_result::overflow));
        int64_t value;

        CHECK(parse_integral<int64_t>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }

    SECTION("parse uint64_t")
    {
        auto data = GENERATE(
            testdata("0", uint64_t(0), parse_integral_result::success),
            testdata("18446744073709551615", uint64_t(0xffffffffffffffff), parse_integral_result::success),
            testdata("18446744073709551616", uint64_t(0xffffffffffffffff), parse_integral_result::overflow));
        uint64_t value;

        CHECK(parse_integral<uint64_t>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }

    SECTION("parse int32_t")
    {
        auto data = GENERATE(
            testdata("-2147483649", int32_t(0x80000000), parse_integral_result::underflow),
            testdata("-2147483648", int32_t(0x80000000), parse_integral_result::success),
            testdata(" 2147483647", int32_t(0x7fffffff), parse_integral_result::success),
            testdata(" 2147483648", int32_t(0x7fffffff), parse_integral_result::overflow));
        int32_t value;

        CHECK(parse_integral<int32_t>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }

    SECTION("parse uint32_t")
    {
        auto data = GENERATE(
            testdata("0", uint32_t(0), parse_integral_result::success),
            testdata("4294967295", uint32_t(0xffffffff), parse_integral_result::success),
            testdata("4294967296", uint32_t(0xffffffff), parse_integral_result::overflow));
        uint32_t value;

        CHECK(parse_integral<uint32_t>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }

    SECTION("parse int8_t")
    {
        auto data = GENERATE(
            testdata("-129", -128, parse_integral_result::underflow),
            testdata("-128", -128, parse_integral_result::success),
            testdata("127", 127, parse_integral_result::success),
            testdata("128", 127, parse_integral_result::overflow));
        int8_t value;

        CHECK(parse_integral<int8_t>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }

    SECTION("parse uint8_t")
    {
        auto data = GENERATE(
            testdata("0", 0, parse_integral_result::success),
            testdata("255", 255, parse_integral_result::success),
            testdata("256", 255, parse_integral_result::overflow));
        uint8_t value;

        CHECK(parse_integral<uint8_t>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }

    SECTION("garbage input")
    {
        auto data = GENERATE(
            testdata("", 0, parse_integral_result::invalid_numeric_string),
            testdata(" ", 0, parse_integral_result::invalid_numeric_string),
            testdata("!", 0, parse_integral_result::invalid_numeric_string),
            testdata("!?", 0, parse_integral_result::invalid_numeric_string),
            testdata("!5", 0, parse_integral_result::invalid_numeric_string),
            testdata("5x", 5, parse_integral_result::trailing_garbage),
            testdata("5 x", 5, parse_integral_result::trailing_garbage));
        long value;

        CHECK(parse_integral<long>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }

    SECTION("leading and trailing whitespace")
    {
        auto data = GENERATE(
            testdata(" 5", 5, parse_integral_result::success),
            testdata(" 5 ", 5, parse_integral_result::success),
            testdata(" 5 \t\n", 5, parse_integral_result::success));
        long value;

        CHECK(parse_integral<long>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }
}

// TODO: redo stuff below
/*
// TODO: probably silly, but this actually does compile for TValue=bool
//       Question is, probably the range check should be handled specially.
//       There should be NO range check. Instead, it should convert zero to false and nonzero to true.
//       That, or it should not support bool at all.
template <std::integral TValue>
bool parse_integral(const char* s, TValue& result, int base)
{
    char* end;
    auto tmp = argpppp::string_to_integral_converter<TValue>::convert(s, &end, base);

    // TODO: this is missing *all* error handling
    // TODO: missing: error/range check of strtoll/strtol: look at result, errno (and end?)
    // TODO: missing: is there junk at end of input: look at end

    if constexpr (sizeof(decltype(tmp)) <= sizeof(TValue))
    {
        result = tmp;
    }
    else
    {
        // TODO: missing: we're converting from long to something smaller, so we need an additional range check. For starters we simply cast,
        //       but we really need a range check
        result = static_cast<TValue>(tmp);
    }

    return true;
}

}

TEST_CASE("parse_number_prototyping")
{
    bool b;
    long double ld;
    double d;
    float f;

    CHECK(parse_integral("23", b, 10) == true);
    CHECK(b == true);

    CHECK(parse_integral("0", b, 10) == true);
    CHECK(b == false);

    CHECK(parse_integral("1", b, 10) == true);
    CHECK(b == true);

    CHECK(parse_floating_point("0.03125", ld) == true);
    CHECK(ld == 0.03125);

    CHECK(parse_floating_point("0.125", d) == true);
    CHECK(d == 0.125);

    CHECK(parse_floating_point("0.25", f) == true);
    CHECK(f == 0.25f);
}*/

}