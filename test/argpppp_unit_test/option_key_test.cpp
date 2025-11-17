// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <limits>
#include <utility>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::option_key;
using std::make_pair;

TEST_CASE("option_key")
{
    SECTION("is_printable")
    {
        auto [c, expected] = GENERATE(
            make_pair(std::numeric_limits<char>::min(), false),
            make_pair(-1, false),
            make_pair(0, false),
            make_pair(31, false),
            make_pair(32, true),
            make_pair(126, true),
            make_pair(127, false),
            make_pair(std::numeric_limits<char>::max(), false)
        );

        CHECK(option_key(c).is_printable() == expected);
    }

    SECTION("requires_long_name")
    {
        auto [c, expected] = GENERATE(
            make_pair(char(0), false),
            make_pair(1, true),
            make_pair('a', false));

        CHECK(option_key(c).requires_long_name() == expected);
    }
}

}
