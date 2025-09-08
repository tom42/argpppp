// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <limits>

import argpppp;

namespace argpppp_unit_test
{

using options = argpppp::options;

TEST_CASE("options_test")
{
    options options;

    SECTION("state after construction")
    {
        CHECK(options.doc().has_value() == false);
        CHECK(options.args_doc().has_value() == false);
        CHECK(options.min_args() == 0);
        CHECK(options.max_args() == std::numeric_limits<std::size_t>::max());
    }

    SECTION("set program documentation")
    {
        options.doc("Program documentation");
        CHECK(options.doc() == "Program documentation");
    }

    SECTION("set argument documentation")
    {
        options.args_doc("Argument documentation");
        CHECK(options.args_doc() == "Argument documentation");
    }
}

}
