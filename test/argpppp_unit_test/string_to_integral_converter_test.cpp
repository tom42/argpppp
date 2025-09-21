// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <type_traits>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::string_to_integral_converter;

inline constexpr int auto_detect_base = 0;

TEST_CASE("string_to_integral_converter_test, long long")
{
    char* end;
    auto result = string_to_integral_converter<long long>::convert("-077!", &end, auto_detect_base);

    CHECK(std::is_same_v<long long, decltype(result)>);
    CHECK(result == -63);
    CHECK(*end == '!');
}

TEMPLATE_TEST_CASE("string_to_integral_converter_test, signed types more narrow than long long", "", long, int, short, signed char)
{
    char* end;
    auto result = string_to_integral_converter<TestType>::convert("-077!", &end, auto_detect_base);

    CHECK(std::is_same_v<long, decltype(result)>);
    CHECK(result == -63);
    CHECK(*end == '!');
}

TEST_CASE("string_to_integral_converter_test, unsigned long long")
{
    char* end;
    auto result = string_to_integral_converter<unsigned long long>::convert("0377!", &end, auto_detect_base);

    CHECK(std::is_same_v<unsigned long long, decltype(result)>);
    CHECK(result == 255);
    CHECK(*end == '!');
}

TEMPLATE_TEST_CASE("string_to_integral_converter_test, unsigned types more narrow than unsigned long long", "", unsigned long, unsigned int, unsigned short, unsigned char)
{
    char* end;
    auto result = string_to_integral_converter<TestType>::convert("0377!", &end, auto_detect_base);

    CHECK(std::is_same_v<unsigned long, decltype(result)>);
    CHECK(result == 255);
    CHECK(*end == '!');
}

}
