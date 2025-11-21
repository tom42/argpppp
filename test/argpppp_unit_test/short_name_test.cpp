// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <optional>
#include <stdexcept>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::short_name;

TEST_CASE("short_name")
{
    SECTION("empty short name")
    {
        short_name s;

        CHECK(s.is_empty() == true);
        CHECK(s.is_null() == false);
        CHECK(s.is_printable() == false);
        CHECK_THROWS_AS(s.to_char(), std::bad_optional_access);
    }

    SECTION("printable character")
    {
        short_name s('a');

        CHECK(s.is_empty() == false);
        CHECK(s.is_null() == false);
        CHECK(s.is_printable() == true);
        CHECK(s.to_char() == 'a');
    }

    SECTION("non-printable character")
    {
        CHECK_THROWS_MATCHES(
            short_name(1),
            std::invalid_argument,
            Catch::Matchers::Message("short name must be a printable character or NUL"));
    }

    SECTION("null")
    {
        auto s = short_name::null();

        CHECK(s.is_empty() == false);
        CHECK(s.is_null() == true);
        CHECK(s.is_printable() == false);
        CHECK(s.to_char() == '\0');
    }
}

}
