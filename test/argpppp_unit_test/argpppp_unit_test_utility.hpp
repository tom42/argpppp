// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#ifndef ARGPPPP_UNIT_TEST_UTILITY_HPP_20250924
#define ARGPPPP_UNIT_TEST_UTILITY_HPP_20250924

#include <algorithm>
#include <concepts>

namespace argpppp_unit_test
{

template <std::floating_point TFloatingPoint>
bool float_equal_no_warning(TFloatingPoint a, TFloatingPoint b)
{
    // Compare floating point values for equality without warning, even in the presense of -Wfloat-equal.
    // Takes advantage of the fact that warnings are not enabled for library headers.
    return std::equal_to<TFloatingPoint>()(a, b);
}

}

#endif
