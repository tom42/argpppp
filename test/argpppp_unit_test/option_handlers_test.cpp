// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <cstdint>

import argpppp;

namespace argpppp_unit_test
{

TEST_CASE("value<std::signed_integral>")
{
    int32_t target;
    argpppp::value<int32_t> value(target);

    // TODO: test successful parsing with default settings
    // TODO: test too small/too big => appropriate error message
    // TODO: test bad input => appropriate error message
    // TODO: test alternate base setting (e.g. 0 for autodetect and maybe some nonstandard base such as 6 or something)
}

}
