// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

import argpppp;

namespace argpppp_unit_test
{

namespace fs = std::filesystem;
using argpppp::error;
using argpppp::ok;
using argpppp::option_handler_result;
using argpppp::option_occurrence;
using std::make_pair;
using std::optional;
using std::string;

TEMPLATE_TEST_CASE("value<convertible from const char*>", "", fs::path, optional<fs::path>, string, optional<string>)
{
    TestType target;
    argpppp::option opt('o', {}, {}, "VALUE");
    argpppp::value value(target);

    SECTION("successful parsing")
    {
        CHECK(value.handle_option(option_occurrence(opt, "arg")) == ok());
        CHECK(target == "arg");
    }

    SECTION("optional arguments are not supported")
    {
        CHECK_THROWS_MATCHES(
            value.handle_option(option_occurrence(opt, nullptr)),
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
        target = false;

        CHECK(value.handle_option(option_occurrence(switch_opt, nullptr)) == ok());
        CHECK(target == true);
    }

    SECTION("successful parsing, negative logic")
    {
        target = true;
        value.negative();

        CHECK(value.handle_option(option_occurrence(switch_opt, nullptr)) == ok());
        CHECK(target == false);
    }

    SECTION("handling arguments is not supported")
    {
        CHECK_THROWS_MATCHES(
            value.handle_option(option_occurrence(switch_opt, "arg")),
            std::logic_error,
            Catch::Matchers::Message("arguments are not supported. value<bool> should be used for switches only"));
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
