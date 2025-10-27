// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
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
    // TODO: implement
    //       * successful parsing (own section)
    //         * Test more than one type of callable?
    //         * Test that return type is ignored? (Then again - why bother?)
    //       * optional arguments are not supported (own section)

    SECTION("successful parsing")
    {
        CHECK(set.handle_option(opt, "arg") == ok());
        CHECK(s == "arg");
    }
}

TEST_CASE("set<bool>")
{
    // TODO: implement
}

TEST_CASE("set<signed_integral>")
{
    // TODO: implement
}

}
