// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <limits>
#include <utility>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::need_long_name;
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

    SECTION("need_long_name")
    {
        CHECK(need_long_name(0) == false);
        CHECK(need_long_name(1) == true);
        CHECK(need_long_name('a') == false);
    }
}

}
