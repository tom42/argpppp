// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::float_equal_no_warning;

TEMPLATE_TEST_CASE("interval", "", int, double)
{
    SECTION("constructor")
    {
        argpppp::interval<TestType> interval;

        CHECK(float_equal_no_warning(interval.min(), std::numeric_limits<TestType>::min()));
        CHECK(float_equal_no_warning(interval.max(), std::numeric_limits<TestType>::max()));
    }
}

}
