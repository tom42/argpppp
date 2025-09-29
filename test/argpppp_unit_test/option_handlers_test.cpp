// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <limits>
#include <utility>
#include <variant>

import argpppp;

namespace argpppp_unit_test
{

using option_error = argpppp::option_error;

TEST_CASE("value<std::signed_integral>")
{
    constexpr int32_t default_target_value = std::numeric_limits<int32_t>::max();
    constexpr int32_t custom_min = 0;
    constexpr int32_t custom_max = 10;
    int32_t target = default_target_value;
    argpppp::value<int32_t> value(target);

    SECTION("successful parsing with default settings")
    {
        CHECK(std::get<bool>(value.handle_option("-2147483648")) == true);
        CHECK(target == -2147483648);

        CHECK(std::get<bool>(value.handle_option("2147483647")) == true);
        CHECK(target == 2147483647);

        // TODO: do we need to test that base is 10 by default and not auto-detect?
    }

    SECTION("successful parsing with custom minimum and maximum value")
    {
        value.min(custom_min).max(custom_max);

        CHECK(std::get<bool>(value.handle_option("0")) == true);
        CHECK(target == 0);

        CHECK(std::get<bool>(value.handle_option("10")) == true);
        CHECK(target == 10);
    }

    // TODO: actually must also test this if it is flagged by strtoll!
    SECTION("parsed value is out of range")
    {
        value.min(custom_min).max(custom_max);

        CHECK(std::get<option_error>(value.handle_option("-1")) == option_error("value should be in range [0, 10]"));
        CHECK(target == default_target_value);

        CHECK(std::get<option_error>(value.handle_option("11")) == option_error("value should be in range [0, 10]"));
        CHECK(target == default_target_value);
    }

    SECTION("successful parsing with auto-detection of base")
    {
        value.auto_detect_base();

        CHECK(std::get<bool>(value.handle_option("010")) == true);
        CHECK(target == 8);

        CHECK(std::get<bool>(value.handle_option("0x10")) == true);
        CHECK(target == 16);
    }

    SECTION("successful parsing with non-standard base")
    {
        value.base(6);

        CHECK(std::get<bool>(value.handle_option("20")) == true);
        CHECK(target == 12);
    }

    SECTION("garbage input")
    {
        CHECK(std::get<option_error>(value.handle_option("invalid number")) == option_error("meh"));
        CHECK(target == default_target_value);
    }
}

}
