// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <utility>
#include <variant>

import argpppp;

namespace argpppp_unit_test
{

using option_error = argpppp::option_error;

TEST_CASE("value<std::signed_integral>")
{
    int32_t target;
    argpppp::value<int32_t> value(target);

    SECTION("successful parsing with default settings")
    {
        CHECK(std::get<bool>(value.handle_option("-2147483648")) == true);
        CHECK(target == -2147483648);

        CHECK(std::get<bool>(value.handle_option("2147483647")) == true);
        CHECK(target == 2147483647);

        // TODO: do we need to test that base is 10 by default and not auto-detect?
    }

    SECTION("parsed value is out of range")
    {
        value.min(0).max(10);

        CHECK(std::get<option_error>(value.handle_option("-1")) == option_error("meh"));
        CHECK(target == -1);

        // TODO: check parsing 10 fails with error message
        // TODO: do we need to check 0 and 10 pass? (in principle yes)
    }

    SECTION("successful parsing with auto-detection of base")
    {
        value.auto_detect_base();

        CHECK(std::get<bool>(value.handle_option("010")) == true);
        CHECK(target == 8);

        CHECK(std::get<bool>(value.handle_option("0x10")) == true);
        CHECK(target == 16);
    }

    // TODO: test too small/too big => appropriate error message
    // TODO: test bad input => appropriate error message
    // TODO: test alternate base setting (e.g. 0 for autodetect and maybe some nonstandard base such as 6 or something)
}

}
