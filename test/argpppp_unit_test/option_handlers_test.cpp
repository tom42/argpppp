// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <limits>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::error;
using argpppp::ok;
using argpppp::option_handler_result;

TEST_CASE("value<std::signed_integral>")
{
    constexpr int16_t default_target_value = std::numeric_limits<int16_t>::max();
    constexpr int16_t custom_min = 0;
    constexpr int16_t custom_max = 10;
    int16_t target = default_target_value;
    argpppp::option option('i', {}, {}, "INTEGER");
    argpppp::value<int16_t> value(target);

    SECTION("successful parsing with default settings")
    {
        CHECK(value.handle_option("-32768", option) == ok());
        CHECK(target == -32768);

        CHECK(value.handle_option("32767", option) == ok());
        CHECK(target == 32767);
    }

    SECTION("successful parsing with custom minimum and maximum value")
    {
        value.min(custom_min).max(custom_max);

        CHECK(value.handle_option("0", option) == ok());
        CHECK(target == 0);

        CHECK(value.handle_option("10", option) == ok());
        CHECK(target == 10);
    }

    SECTION("parsed value is out of range, custom minimum and maximum value")
    {
        value.min(custom_min).max(custom_max);

        CHECK(value.handle_option("-1", option) == error("invalid argument '-1' for option '-i': value must be in range [0, 10]"));
        CHECK(target == default_target_value);

        CHECK(value.handle_option("11", option) == error("invalid argument '11' for option '-i': value must be in range [0, 10]"));
        CHECK(target == default_target_value);
    }

    SECTION("parsed value is out of range, range limited by type")
    {
        CHECK(value.handle_option("-32769", option) == error("invalid argument '-32769' for option '-i': value must be in range [-32768, 32767]"));
        CHECK(target == default_target_value);

        CHECK(value.handle_option("32768", option) == error("invalid argument '32768' for option '-i': value must be in range [-32768, 32767]"));
        CHECK(target == default_target_value);
    }

    SECTION("successful parsing with auto-detection of base")
    {
        value.auto_detect_base();

        CHECK(value.handle_option("010", option) == ok());
        CHECK(target == 8);

        CHECK(value.handle_option("0x10", option) == ok());
        CHECK(target == 16);
    }

    SECTION("successful parsing with non-standard base")
    {
        value.base(6);

        CHECK(value.handle_option("20", option) == ok());
        CHECK(target == 12);
    }

    SECTION("garbage input")
    {
        CHECK(value.handle_option("!?*", option) == error("invalid argument '!?*' for option '-i': not a valid integer number"));
        CHECK(target == default_target_value);
    }

    SECTION("auto-detection of base is off by default")
    {
        CHECK(value.handle_option("0x10", option) == error("invalid argument '0x10' for option '-i': not a valid integer number"));
        CHECK(target == default_target_value);
    }
}

}
