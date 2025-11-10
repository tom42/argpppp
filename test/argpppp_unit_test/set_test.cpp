// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::error;
using argpppp::ok;
using string = std::string;
using std::make_pair;

TEST_CASE("set<string>")
{
    string s;
    argpppp::option opt('s', {}, {}, "STRING");
    argpppp::set<string> set([&s](string arg) { s = arg; });

    SECTION("successful parsing")
    {
        CHECK(set.handle_option(opt, "arg") == ok());
        CHECK(s == "arg");
    }

    SECTION("optional arguments are not supported")
    {
        CHECK_THROWS_MATCHES(
            set.handle_option(opt, nullptr),
            std::logic_error,
            Catch::Matchers::Message("optional arguments are currently not supported"));
    }
}

TEST_CASE("set<bool>")
{
    bool flag = false;
    argpppp::option switch_opt('s');
    argpppp::set<bool> set([&flag](bool arg) { flag = arg; });

    SECTION("successful parsing")
    {
        CHECK(set.handle_option(switch_opt, nullptr) == ok());
        CHECK(flag == true);
    }

    SECTION("handling arguments is not supported")
    {
        CHECK_THROWS_MATCHES(
            set.handle_option(switch_opt, "arg"),
            std::logic_error,
            Catch::Matchers::Message("arguments are not supported. set<bool> should be used for switches only"));
    }
}

TEST_CASE("set<signed_integral>")
{
    std::optional<int> i;
    argpppp::option opt('i', {}, {}, "INTEGER");
    argpppp::set<int> set([&i](int arg) { i = arg; });

    SECTION("successful parsing, default base")
    {
        CHECK(set.handle_option(opt, "10") == ok());
        CHECK(i == 10);
    }

    SECTION("successful parsing, non-default base")
    {
        set.base(36);

        CHECK(set.handle_option(opt, "10") == ok());
        CHECK(i == 36);
    }

    SECTION("successful parsing, detect base automatically")
    {
        auto [arg, expected_value] = GENERATE(
            make_pair("123", 123),
            make_pair("0x123", 0x123));
        set.auto_detect_base();

        CHECK(set.handle_option(opt, arg) == ok());
        CHECK(i == expected_value);
    }

    SECTION("successful parsing, in range")
    {
        auto [arg, expected_value] = GENERATE(
            make_pair("1", 1),
            make_pair("10", 10));
        set.min(1).max(10);

        CHECK(set.handle_option(opt, arg) == ok());
        CHECK(i == expected_value);
    }

    SECTION("failed parsing, out of range")
    {
        auto [arg, expected_error_message] = GENERATE(
            make_pair("0", "invalid argument '0' for option '-i': value must be in range [1, 10]"),
            make_pair("11", "invalid argument '11' for option '-i': value must be in range [1, 10]"));
        set.min(1).max(10);

        CHECK(set.handle_option(opt, arg) == error(expected_error_message));
        CHECK(i.has_value() == false);
    }
}

}
