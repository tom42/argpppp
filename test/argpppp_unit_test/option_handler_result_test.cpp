// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <string>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::error;
using argpppp::ok;

TEST_CASE("option_handler_result")
{
    argpppp::option option('i', {}, {} , "INTEGER");

    SECTION("ok")
    {
        const auto result = ok();

        CHECK(result.is_success() == true);
        CHECK(result.exit_status() == EXIT_SUCCESS);
        CHECK(result.error_number() == 0);
        CHECK(result.error_message() == "");
        CHECK(result.include_standard_error_message() == false);
    }

    SECTION("error, overload taking single error message")
    {
        const auto result = error("error message");

        CHECK(result.is_success() == false);
        CHECK(result.exit_status() == EXIT_FAILURE);
        CHECK(result.error_number() == EINVAL);
        CHECK(result.error_message() == "error message");
        CHECK(result.include_standard_error_message() == false);
    }

    SECTION("error, overload taking option, argument and error message as std::string")
    {
        const auto result = error(option, "123", std::string("value is too big"));

        CHECK(result.is_success() == false);
        CHECK(result.exit_status() == EXIT_FAILURE);
        CHECK(result.error_number() == EINVAL);
        CHECK(result.error_message() == "invalid argument '123' for option '-i': value is too big");
        CHECK(result.include_standard_error_message() == false);
    }

    SECTION("error, overload taking option, argument and error message as const char*")
    {
        const auto result = error(option, "123", "value is too big");

        CHECK(result.is_success() == false);
        CHECK(result.exit_status() == EXIT_FAILURE);
        CHECK(result.error_number() == EINVAL);
        CHECK(result.error_message() == "invalid argument '123' for option '-i': value is too big");
        CHECK(result.include_standard_error_message() == false);
    }
}

}
