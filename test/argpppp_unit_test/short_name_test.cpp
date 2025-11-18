// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <stdexcept>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::short_name;

TEST_CASE("short_name")
{
    SECTION("constructor, empty short_name")
    {
        CHECK(short_name().is_empty() == true);
    }

    SECTION("constructor, non-empty short_name")
    {
        char c = GENERATE(char(0), 32, 126);

        CHECK(short_name(c).is_empty() == false);
    }

    SECTION("constructor throws if non-printable character is specified")
    {
        char c = GENERATE(char(1), 31, 127);

        CHECK_THROWS_MATCHES(
            short_name(c),
            std::invalid_argument,
            Catch::Matchers::Message("short name must be a printable character or NUL"));
    }

    SECTION("null")
    {
        CHECK(short_name::null() == short_name(0));
    }
}

}
