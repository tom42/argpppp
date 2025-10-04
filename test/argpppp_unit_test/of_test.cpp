// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::of;

TEST_CASE("of")
{
    SECTION("bitwise and")
    {
        constexpr auto result = of::alias & of::arg_optional;
        CHECK(result == of(0));
        CHECK((of::arg_optional & of::arg_optional) == of(1));
    }

    SECTION("bitwise or")
    {
        constexpr auto result = of::arg_optional | of::alias;
        CHECK(result == of(5));
    }

    SECTION("to_int")
    {
        constexpr auto result = to_int(of::no_usage);
        CHECK(result == 16);
    }
}

}
