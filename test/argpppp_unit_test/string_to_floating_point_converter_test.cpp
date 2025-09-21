// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::string_to_floating_point_converter;

TEST_CASE("string_to_floating_point_converter_test")
{
    SECTION("float")
    {
        char* end;
        auto result = string_to_floating_point_converter<float>::convert("0.25!", &end);

        CHECK(std::is_same_v<float, decltype(result)>);
        CHECK(result == 0.25f);
        CHECK(*end == '!');
    }
}

}
