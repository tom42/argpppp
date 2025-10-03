// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

import argpppp;

namespace argpppp_unit_test
{

using option_handler_result = argpppp::option_handler_result;

TEST_CASE("option_handler_result")
{
    SECTION("success")
    {
        const auto result = option_handler_result::success();

        CHECK(result.is_success() == true);
        CHECK(result.exit_status() == EXIT_SUCCESS);
        CHECK(result.error_number() == 0);
        CHECK(result.error_message() == "");
    }

    SECTION("error")
    {
        const auto result = option_handler_result::error("error message");

        CHECK(result.is_success() == false);
        CHECK(result.exit_status() == EXIT_FAILURE);
        CHECK(result.error_number() == EINVAL);
        CHECK(result.error_message() == "error message");
    }
}

}
