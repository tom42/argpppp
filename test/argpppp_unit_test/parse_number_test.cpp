// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <type_traits>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::float_equal_no_warning;
using argpppp::parse_floating_point;
using argpppp::parse_integral;
using argpppp::parse_integral_result;

namespace
{

template <typename TValue>
struct testdata final
{
    std::string input;
    TValue expected_value;
    parse_integral_result expected_parse_result;
};

}

TEST_CASE("parse_integral_test")
{
    SECTION("parse int64_t")
    {
        auto data = GENERATE(
            testdata<int64_t>{"-9223372036854775809", int64_t(0x8000000000000000), parse_integral_result::underflow},
            testdata<int64_t>{"-9223372036854775808", int64_t(0x8000000000000000), parse_integral_result::success},
            testdata<int64_t>{" 9223372036854775807", 0x7fffffffffffffff, parse_integral_result::success},
            testdata<int64_t>{" 9223372036854775808", 0x7fffffffffffffff, parse_integral_result::overflow});
        int64_t value;

        CHECK(parse_integral<int64_t>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }

    SECTION("parse uint64_t")
    {
        auto data = GENERATE(
            testdata<uint64_t>{"0", 0, parse_integral_result::success},
            testdata<uint64_t>{"18446744073709551615", 0xffffffffffffffff, parse_integral_result::success},
            testdata<uint64_t>{"18446744073709551616", 0xffffffffffffffff, parse_integral_result::overflow});
        uint64_t value;

        CHECK(parse_integral<uint64_t>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }

    SECTION("parse int32_t")
    {
        auto data = GENERATE(
            testdata<int32_t>{"-2147483649", int32_t(0x80000000), parse_integral_result::underflow},
            testdata<int32_t>{"-2147483648", int32_t(0x80000000), parse_integral_result::success},
            testdata<int32_t>{" 2147483647", 0x7fffffff, parse_integral_result::success},
            testdata<int32_t>{" 2147483648", 0x7fffffff, parse_integral_result::overflow});
        int32_t value;

        CHECK(parse_integral<int32_t>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }

    SECTION("parse uint32_t")
    {
        auto data = GENERATE(
            testdata<uint32_t>{"0", 0, parse_integral_result::success},
            testdata<uint32_t>{"4294967295", 0xffffffff, parse_integral_result::success},
            testdata<uint32_t>{"4294967296", 0xffffffff, parse_integral_result::overflow});
        uint32_t value;

        CHECK(parse_integral<uint32_t>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }

    SECTION("parse int8_t")
    {
        auto data = GENERATE(
            testdata{"-129", -128, parse_integral_result::underflow},
            testdata{"-128", -128, parse_integral_result::success},
            testdata{"127", 127, parse_integral_result::success},
            testdata{"128", 127, parse_integral_result::overflow});
        int8_t value;

        CHECK(parse_integral<int8_t>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }

    SECTION("parse uint8_t")
    {
        auto data = GENERATE(
            testdata{"0", 0, parse_integral_result::success},
            testdata{"255", 255, parse_integral_result::success},
            testdata{"256", 255, parse_integral_result::overflow});
        uint8_t value;

        CHECK(parse_integral<uint8_t>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }

    SECTION("garbage input")
    {
        auto data = GENERATE(
            testdata{"", 0, parse_integral_result::invalid_numeric_string},
            testdata{" ", 0, parse_integral_result::invalid_numeric_string},
            testdata{"!", 0, parse_integral_result::invalid_numeric_string},
            testdata{"!?", 0, parse_integral_result::invalid_numeric_string},
            testdata{"!5", 0, parse_integral_result::invalid_numeric_string},
            testdata{"5x", 5, parse_integral_result::trailing_garbage},
            testdata{"5 x", 5, parse_integral_result::trailing_garbage});
        long value;

        CHECK(parse_integral<long>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }

    SECTION("leading and trailing whitespace")
    {
        auto data = GENERATE(
            testdata{" 5", 5, parse_integral_result::success},
            testdata{" 5 ", 5, parse_integral_result::success},
            testdata{" 5 \t\n", 5, parse_integral_result::success});
        long value;

        CHECK(parse_integral<long>(data.input, value, 10) == data.expected_parse_result);
        CHECK(value == data.expected_value);
    }

    SECTION("base other than 10")
    {
        int value;
        CHECK(parse_integral("20", value, 16) == parse_integral_result::success);
        CHECK(value == 32);
    }

    SECTION("valid base")
    {
        int base = GENERATE(0, 2, 36);

        int value;
        CHECK(parse_integral("1", value, base) == parse_integral_result::success);
        CHECK(value == 1);
    }

    SECTION("invalid base")
    {
        int base = GENERATE(-1, 1, 37);

        int value;
        CHECK_THROWS_MATCHES(
            parse_integral("", value, base),
            std::invalid_argument,
            Catch::Matchers::Message("parse_integral: invalid base"));
    }

    SECTION("base is forwarded to strtoxx")
    {
        int value;

        CHECK(parse_integral("0x10", value, 0) == parse_integral_result::success);
        CHECK(value == 16);
    }
}

// TODO: add tests
//       * parse long double
//       * parse float
//       * garbage input (leading/trailing junk)
//       * leading/trailing whitespace
//       * anything else?
TEST_CASE("parse_floating_point_test")
{
    SECTION("parse double")
    {
        auto data = GENERATE(
            testdata<double>{"-1e1000", -HUGE_VAL, parse_integral_result::underflow},
            testdata<double>{"0.25", 0.25, parse_integral_result::success},
            testdata<double>{"0.5", 0.5, parse_integral_result::success},
            testdata<double>{"1e1000", HUGE_VAL, parse_integral_result::overflow});

        double value;
        CHECK(parse_floating_point<double>(data.input, value) == data.expected_parse_result);
        CHECK(float_equal_no_warning(value, data.expected_value));
    }

    SECTION("parse float")
    {
        auto data = GENERATE(
            testdata<float>{"-1e1000", -HUGE_VALF, parse_integral_result::underflow},
            testdata<float>{"0.25", 0.25f, parse_integral_result::success},
            testdata<float>{"0.5", 0.5f, parse_integral_result::success},
            testdata<float>{"1e1000", HUGE_VALF, parse_integral_result::overflow});

        float value;
        CHECK(parse_floating_point<float>(data.input, value) == data.expected_parse_result);
        CHECK(float_equal_no_warning(value, data.expected_value));
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
    // ...
}

}

TEST_CASE("parse_number_prototyping_test")
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
