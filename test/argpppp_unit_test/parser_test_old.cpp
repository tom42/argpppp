// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <cerrno>
#include <cstring>
#include <functional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

import argpppp;

namespace argpppp_unit_test
{

using argpppp_old::parser;

TEST_CASE("parser_test_old")
{
    parser parser;

    SECTION("set_nargs throws if min is > max")
    {
        CHECK_THROWS_MATCHES(
            parser.set_nargs(3, 2),
            std::logic_error,
            Catch::Matchers::Message("min_args must be <= max_args"));
    }
}

}
