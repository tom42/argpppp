// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <climits>
#include <type_traits>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::string_to_integral_converter;

inline constexpr int auto_detect_base = 0;

TEST_CASE("string_to_integral_converter, long long")
{
    char* end;
    auto result = string_to_integral_converter<long long>::convert("-077!", &end, auto_detect_base);

    CHECK(std::is_same_v<decltype(result), long long>);
    CHECK(result == -63);
    CHECK(*end == '!');
    CHECK(string_to_integral_converter<long long>::min() == LLONG_MIN);
    CHECK(string_to_integral_converter<long long>::max() == LLONG_MAX);
    CHECK(std::is_same_v<decltype(string_to_integral_converter<long long>::min()), long long>);
    CHECK(std::is_same_v<decltype(string_to_integral_converter<long long>::max()), long long>);
}

TEMPLATE_TEST_CASE("string_to_integral_converter, signed types more narrow than long long", "", long, int, short, signed char)
{
    char* end;
    auto result = string_to_integral_converter<TestType>::convert("-077!", &end, auto_detect_base);

    CHECK(std::is_same_v<long, decltype(result)>);
    CHECK(result == -63);
    CHECK(*end == '!');
    CHECK(string_to_integral_converter<TestType>::min() == LONG_MIN);
    CHECK(string_to_integral_converter<TestType>::max() == LONG_MAX);
    CHECK(std::is_same_v<decltype(string_to_integral_converter<TestType>::min()), long>);
    CHECK(std::is_same_v<decltype(string_to_integral_converter<TestType>::max()), long>);
}

TEST_CASE("string_to_integral_converter, unsigned long long")
{
    char* end;
    auto result = string_to_integral_converter<unsigned long long>::convert("0377!", &end, auto_detect_base);

    CHECK(std::is_same_v<unsigned long long, decltype(result)>);
    CHECK(result == 255);
    CHECK(*end == '!');
    CHECK(string_to_integral_converter<unsigned long long>::min() == 0);
    CHECK(string_to_integral_converter<unsigned long long>::max() == ULLONG_MAX);
    CHECK(std::is_same_v<decltype(string_to_integral_converter<unsigned long long>::min()), unsigned long long>);
    CHECK(std::is_same_v<decltype(string_to_integral_converter<unsigned long long>::max()), unsigned long long>);
}

TEMPLATE_TEST_CASE("string_to_integral_converter, unsigned types more narrow than unsigned long long", "", unsigned long, unsigned int, unsigned short, unsigned char)
{
    char* end;
    auto result = string_to_integral_converter<TestType>::convert("0377!", &end, auto_detect_base);

    CHECK(std::is_same_v<unsigned long, decltype(result)>);
    CHECK(result == 255);
    CHECK(*end == '!');
    CHECK(string_to_integral_converter<TestType>::min() == 0);
    CHECK(string_to_integral_converter<TestType>::max() == ULONG_MAX);
    CHECK(std::is_same_v<decltype(string_to_integral_converter<TestType>::min()), unsigned long>);
    CHECK(std::is_same_v<decltype(string_to_integral_converter<TestType>::max()), unsigned long>);
}

}
