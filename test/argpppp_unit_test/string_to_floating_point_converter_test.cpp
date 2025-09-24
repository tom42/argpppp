// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <concepts>
#include <type_traits>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::string_to_floating_point_converter;

namespace
{

template <std::floating_point TFloatingPoint>
bool float_equal_no_warning(TFloatingPoint a, TFloatingPoint b)
{
    // Compare floating point values for equality without warning, even in the presense of -Wfloat-equal.
    // Takes advantage of the fact that warnings are not enabled for library headers.
    return std::equal_to<TFloatingPoint>()(a, b);
}

}

TEMPLATE_TEST_CASE("string_to_floating_point_converter_test", "", float, double, long double)
{
    char* end;
    auto result = string_to_floating_point_converter<TestType>::convert("0.25!", &end);

    CHECK(std::is_same_v<TestType, decltype(result)>);
    CHECK(float_equal_no_warning(result, TestType(0.25)));
    CHECK(*end == '!');
}

}
