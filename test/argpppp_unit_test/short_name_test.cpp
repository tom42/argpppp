// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::short_name;

TEST_CASE("short_name")
{
    SECTION("constructor, empty short_name")
    {
        short_name empty_short_name;

        CHECK(empty_short_name.empty() == true);
    }
}

}
