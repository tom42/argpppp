// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

import argpppp;

namespace argpppp_unit_test
{

// TODO: implement (see value_test<signed_integral>, most of it should go here, probably)
TEST_CASE("signed_integral_argument_parser")
{
}

}

// TODO: see what of va^lue's test below should go into signed integral parsere's test
/*

TEST_CASE("value<signed_integral>")
{
    constexpr int16_t default_target_value = std::numeric_limits<int16_t>::max();
    constexpr int16_t custom_min = 0;
    constexpr int16_t custom_max = 10;
    int16_t target = default_target_value;
    argpppp::option opt('i', {}, {}, "INTEGER");
    argpppp::value value(target);

    SECTION("successful parsing with default settings")
    {
        CHECK(value.handle_option(opt, "-32768") == ok());
        CHECK(target == -32768);

        CHECK(value.handle_option(opt, "32767") == ok());
        CHECK(target == 32767);
    }

    SECTION("successful parsing with custom minimum and maximum value")
    {
        value.min(custom_min).max(custom_max);

        CHECK(value.handle_option(opt, "0") == ok());
        CHECK(target == 0);

        CHECK(value.handle_option(opt, "10") == ok());
        CHECK(target == 10);
    }

    SECTION("parsed value is out of range, custom minimum and maximum value")
    {
        value.min(custom_min).max(custom_max);

        CHECK(value.handle_option(opt, "-1") == error("invalid argument '-1' for option '-i': value must be in range [0, 10]"));
        CHECK(target == default_target_value);

        CHECK(value.handle_option(opt, "11") == error("invalid argument '11' for option '-i': value must be in range [0, 10]"));
        CHECK(target == default_target_value);
    }

    SECTION("parsed value is out of range, range limited by type")
    {
        CHECK(value.handle_option(opt, "-32769") == error("invalid argument '-32769' for option '-i': value must be in range [-32768, 32767]"));
        CHECK(target == default_target_value);

        CHECK(value.handle_option(opt, "32768") == error("invalid argument '32768' for option '-i': value must be in range [-32768, 32767]"));
        CHECK(target == default_target_value);
    }

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
            Catch::Matchers::Message("base: invalid base"));
    }

    SECTION("optional arguments are not supported")
    {
        CHECK_THROWS_MATCHES(
            value.handle_option(opt, nullptr),
            std::logic_error,
            Catch::Matchers::Message("value<std::signed_integral>: optional arguments are currently not supported"));
    }
}

}
*/