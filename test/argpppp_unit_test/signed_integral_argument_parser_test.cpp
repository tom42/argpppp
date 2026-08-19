// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <cstdint>
#include <stdexcept>
#include <string>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::error;
using argpppp::ok;

namespace
{

struct signed_integral_argument_parser_fixture
{
    template <std::signed_integral TValue>
    argpppp::option_handler_result parse_arg(const char* arg, TValue& value)
    {
        return parser.parse_arg(argpppp::option_occurrence(opt, arg), value);
    }

    argpppp::option opt{ 'i', {}, {}, "INTEGER" };
    argpppp::signed_integral_argument_parser<int16_t> parser;
};

}

TEST_CASE_METHOD(signed_integral_argument_parser_fixture, "signed_integral_argument_parser")
{
    constexpr int16_t default_value = 12345;
    constexpr int16_t custom_min = 0;
    constexpr int16_t custom_max = 10;
    int16_t value = default_value;

    SECTION("optional arguments are not supported")
    {
        CHECK_THROWS_MATCHES(
            parse_arg(nullptr, value),
            std::logic_error,
            Catch::Matchers::Message("optional arguments are currently not supported"));
        CHECK(value == default_value);
    }

    SECTION("successful parsing with default settings")
    {
        CHECK(parse_arg("-32768", value) == ok());
        CHECK(value == -32768);

        CHECK(parse_arg("32767", value) == ok());
        CHECK(value == 32767);
    }

    SECTION("successful parsing with custom minimum and maximum value")
    {
        parser.min(custom_min);
        parser.max(custom_max);

        CHECK(parse_arg("0", value) == ok());
        CHECK(value == 0);

        CHECK(parse_arg("10", value) == ok());
        CHECK(value == 10);
    }

    SECTION("parsed value is out of range, custom minimum and maximum value")
    {
        parser.min(custom_min);
        parser.max(custom_max);

        CHECK(parse_arg("-1", value) == error("invalid argument '-1' for option '-i': value must be in range [0, 10]"));
        CHECK(value == default_value);

        CHECK(parse_arg("11", value) == error("invalid argument '11' for option '-i': value must be in range [0, 10]"));
        CHECK(value == default_value);
    }

    SECTION("parsed value is out of range, range limited by type")
    {
        CHECK(parse_arg("-32769", value) == error("invalid argument '-32769' for option '-i': value must be in range [-32768, 32767]"));
        CHECK(value == default_value);

        CHECK(parse_arg("32768", value) == error("invalid argument '32768' for option '-i': value must be in range [-32768, 32767]"));
        CHECK(value == default_value);
    }

    SECTION("successful parsing with auto-detection of base")
    {
        parser.auto_detect_base();

        CHECK(parse_arg("010", value) == ok());
        CHECK(value == 8);

        CHECK(parse_arg("0x10", value) == ok());
        CHECK(value == 16);
    }

    SECTION("successful parsing with non-standard base")
    {
        parser.base(6);

        CHECK(parse_arg("20", value) == ok());
        CHECK(value == 12);
    }

    SECTION("garbage input")
    {
        CHECK(parse_arg("!?*", value) == error("invalid argument '!?*' for option '-i': not a valid integer number"));
        CHECK(value == default_value);
    }

    SECTION("auto-detection of base is off by default")
    {
        CHECK(parse_arg("0x10", value) == error("invalid argument '0x10' for option '-i': not a valid integer number"));
        CHECK(value == default_value);
    }

    SECTION("invalid base")
    {
        CHECK_THROWS_MATCHES(
            parser.base(1),
            std::invalid_argument,
            Catch::Matchers::Message("invalid base"));
    }
}

}
