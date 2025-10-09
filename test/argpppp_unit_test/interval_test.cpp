// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

import argpppp;

namespace argpppp_unit_test
{

template <typename TNumeric>
using interval = argpppp::interval<TNumeric>;

TEMPLATE_TEST_CASE("interval", "", int, double)
{
    SECTION("default constructor")
    {
        interval<TestType> interval;

        CHECK(interval.min() == std::numeric_limits<TestType>::min());
        CHECK(interval.max() == std::numeric_limits<TestType>::max());
    }

    SECTION("constructor with min and max value")
    {
        interval<TestType> interval(1, 5);

        CHECK(interval.min() == TestType(1));
        CHECK(interval.max() == TestType(5));
    }

    SECTION("get and set min")
    {
        interval<TestType> interval;

        interval.min(0);
        interval.max(10);

        CHECK(interval.min() == TestType(0));
        CHECK(interval.max() == TestType(10));
    }

    SECTION("includes")
    {
        interval<TestType> interval(0, 10);

        CHECK(interval.includes(-1) == false);
        CHECK(interval.includes(0) == true);
        CHECK(interval.includes(10) == true);
        CHECK(interval.includes(11) == false);
    }
}

}
