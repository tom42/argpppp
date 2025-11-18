// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::short_name;

TEST_CASE("short_name")
{
    SECTION("constructor, empty short_name")
    {
        CHECK(short_name().is_empty() == true);
    }

    SECTION("constructor, non-empty short_name")
    {
        char c = GENERATE(char(0), 32, 126);

        CHECK(short_name(c).is_empty() == false);
    }
}

}
