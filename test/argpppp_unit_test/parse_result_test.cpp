// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

import argpppp;

namespace argpppp_unit_test
{

using parse_result = argpppp::parse_result;

TEST_CASE("parse_result_test")
{
    SECTION("success")
    {
        CHECK((parse_result{}).success() == true);
        CHECK((parse_result{ 1 }).success() == false);
        CHECK((parse_result{ 2 }).success() == false);
    }
}

}
