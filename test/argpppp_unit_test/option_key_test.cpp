// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <limits>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::is_printable_key;
using argpppp::need_long_name;

TEST_CASE("option_key")
{
    SECTION("is_printable_key")
    {
        CHECK(is_printable_key(std::numeric_limits<int>::min()) == false);
        CHECK(is_printable_key(-1) == false);
        CHECK(is_printable_key(0) == false);
        CHECK(is_printable_key(31) == false);
        CHECK(is_printable_key(32) == true);
        CHECK(is_printable_key(126) == true);
        CHECK(is_printable_key(127) == false);
        CHECK(is_printable_key(std::numeric_limits<int>::max()) == false);
    }

    SECTION("need_long_name")
    {
        CHECK(need_long_name(0) == false);
        CHECK(need_long_name(1) == true);
        CHECK(need_long_name('a') == false);
    }
}

}
