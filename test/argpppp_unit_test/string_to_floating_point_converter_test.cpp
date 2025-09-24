// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <type_traits>
#include "argpppp_unit_test_utility.hpp"

import argpppp;

namespace argpppp_unit_test
{

using argpppp::string_to_floating_point_converter;

TEMPLATE_TEST_CASE("string_to_floating_point_converter_test", "", float, double, long double)
{
    char* end;
    auto result = string_to_floating_point_converter<TestType>::convert("0.25!", &end);

    CHECK(std::is_same_v<TestType, decltype(result)>);
    CHECK(float_equal_no_warning(result, TestType(0.25)));
    CHECK(*end == '!');
}

}
