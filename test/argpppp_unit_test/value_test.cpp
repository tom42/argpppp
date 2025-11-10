// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <cstdint>
#include <limits>
#include <string>
#include <utility>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::error;
using argpppp::ok;
using argpppp::option_handler_result;
using std::string;
using std::make_pair;

TEST_CASE("value<string>")
{
    string target;
    argpppp::option opt('s', {}, {}, "STRING");
    argpppp::value value(target);

    SECTION("successful parsing")
    {
        CHECK(value.handle_option(opt, "arg") == ok());
        CHECK(target == "arg");
    }

    SECTION("optional arguments are not supported")
    {
        CHECK_THROWS_MATCHES(
            value.handle_option(opt, nullptr),
            std::logic_error,
            Catch::Matchers::Message("optional arguments are currently not supported"));
    }
}

TEST_CASE("value<bool>")
{
    bool target = false;
    argpppp::option switch_opt('s');
    argpppp::value value(target);

    SECTION("successful parsing")
    {
        CHECK(value.handle_option(switch_opt, nullptr) == ok());
        CHECK(target == true);
    }

    SECTION("handling arguments is not supported")
    {
        CHECK_THROWS_MATCHES(
            value.handle_option(switch_opt, "arg"),
            std::logic_error,
            Catch::Matchers::Message("arguments are not supported. value<bool> should be used for switches only"));
    }
}

// TODO: review: parts of this should go into new test, parts of this should be covered by signed_integral_argument_parser's test
TEST_CASE("value<signed_integral> (old)")
{
    constexpr int16_t default_target_value = std::numeric_limits<int16_t>::max();
    int16_t target = default_target_value;
    argpppp::option opt('i', {}, {}, "INTEGER");
    argpppp::value value(target);

    SECTION("successful parsing with auto-detection of base")
    {
        value.auto_detect_base();

        CHECK(value.handle_option(opt, "010") == ok());
        CHECK(target == 8);

        CHECK(value.handle_option(opt, "0x10") == ok());
        CHECK(target == 16);
    }

    SECTION("successful parsing with non-standard base")
    {
        value.base(6);

        CHECK(value.handle_option(opt, "20") == ok());
        CHECK(target == 12);
    }

    SECTION("garbage input")
    {
        CHECK(value.handle_option(opt, "!?*") == error("invalid argument '!?*' for option '-i': not a valid integer number"));
        CHECK(target == default_target_value);
    }

    SECTION("auto-detection of base is off by default")
    {
        CHECK(value.handle_option(opt, "0x10") == error("invalid argument '0x10' for option '-i': not a valid integer number"));
        CHECK(target == default_target_value);
    }

    SECTION("invalid base")
    {
        CHECK_THROWS_MATCHES(
            value.base(1),
            std::invalid_argument,
            Catch::Matchers::Message("invalid base"));
    }

    SECTION("optional arguments are not supported")
    {
        CHECK_THROWS_MATCHES(
            value.handle_option(opt, nullptr),
            std::logic_error,
            Catch::Matchers::Message("optional arguments are currently not supported"));
    }
}

TEST_CASE("value<signed_integral>")
{
    constexpr int default_value = 12345;
    int i = default_value;
    argpppp::option opt('i', {}, {}, "INTEGER");
    argpppp::value<int> value(i);

    SECTION("successful parsing, default base")
    {
        CHECK(value.handle_option(opt, "10") == ok());
        CHECK(i == 10);
    }

    SECTION("successful parsing, non-default base")
    {
        value.base(36);

        CHECK(value.handle_option(opt, "10") == ok());
        CHECK(i == 36);
    }

    SECTION("successful parsing, detect base automatically")
    {
        auto [arg, expected_value] = GENERATE(
            make_pair("123", 123),
            make_pair("0x123", 0x123));
        value.auto_detect_base();

        CHECK(value.handle_option(opt, arg) == ok());
        CHECK(i == expected_value);
    }

    SECTION("successful parsing, in range")
    {
        auto [arg, expected_value] = GENERATE(
            make_pair("1", 1),
            make_pair("10", 10));
        value.min(1).max(10);

        CHECK(value.handle_option(opt, arg) == ok());
        CHECK(i == expected_value);
    }

    SECTION("failed parsing, out of range")
    {
        auto [arg, expected_error_message] = GENERATE(
            make_pair("0", "invalid argument '0' for option '-i': value must be in range [1, 10]"),
            make_pair("11", "invalid argument '11' for option '-i': value must be in range [1, 10]"));
        value.min(1).max(10);

        CHECK(value.handle_option(opt, arg) == error(expected_error_message));
        CHECK(i == default_value);
    }
}

}
