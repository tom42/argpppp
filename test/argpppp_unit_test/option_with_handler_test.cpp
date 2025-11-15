// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <stdexcept>

import argpppp;

namespace argpppp_unit_test
{

TEST_CASE("option_with_handler")
{
    SECTION("handle_option throws if there is no handler")
    {
        argpppp::option_with_handler owh({}, {}, {});

        CHECK_THROWS_MATCHES(
            owh.handle_option("arg"),
            std::logic_error,
            Catch::Matchers::Message("no option handler set"));
    }
}

}
