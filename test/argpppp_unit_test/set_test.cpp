// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <stdexcept>
#include <string>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::ok;
using string = std::string;

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
            Catch::Matchers::Message("set<std::string>: optional arguments are currently not supported"));
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
            Catch::Matchers::Message("set<bool>: arguments are not supported. set<bool> should be used for switches only"));
    }
}

TEST_CASE("set<signed_integral>")
{
    // TODO: implement: what tests do we REALLY need?
    int i = 0;
    argpppp::option opt('i', {}, {}, "INTEGER");
    argpppp::set<int> set([&i](int arg) { i = arg; });

    SECTION("successful parsing")
    {
        CHECK(set.handle_option(opt, "123") == ok());
        CHECK(i == 123);
    }
}

}
