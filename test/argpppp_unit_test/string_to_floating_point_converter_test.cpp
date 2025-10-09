// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <cmath>
#include <type_traits>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::string_to_floating_point_converter;

TEMPLATE_TEST_CASE("string_to_floating_point_converter, convert", "", float, double, long double)
{
    char* end;
    auto result = string_to_floating_point_converter<TestType>::convert("0.25!", &end);

    CHECK(std::is_same_v<TestType, decltype(result)>);
    CHECK(result == TestType(0.25));
    CHECK(*end == '!');
}

TEST_CASE("string_to_floating_point_converter, huge_val")
{
    CHECK(string_to_floating_point_converter<long double>::huge_val() == HUGE_VALL);
    CHECK(string_to_floating_point_converter<double>::huge_val() == HUGE_VAL);
    CHECK(string_to_floating_point_converter<float>::huge_val() == HUGE_VALF);
}

}
