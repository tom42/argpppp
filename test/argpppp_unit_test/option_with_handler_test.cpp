// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <stdexcept>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::of;
using argpppp::option;
using argpppp::option_with_handler;

TEST_CASE("option_with_handler")
{
    SECTION("handle_option throws if there is no handler")
    {
        option_with_handler owh({}, {}, {});

        CHECK_THROWS_MATCHES(
            owh.handle_option("arg"),
            std::logic_error,
            Catch::Matchers::Message("no option handler set"));
    }

    SECTION("to_argp_option")
    {
        constexpr int argp_key = 456;
        const option_with_handler owh(
            option('n', "name", "doc", "arg", of::arg_optional, 123),
            argp_key,
            {});

        const auto argp_option = to_argp_option(owh);

        CHECK(argp_option.key == argp_key);
        CHECK(!strcmp(argp_option.name, "name"));
        CHECK(!strcmp(argp_option.doc, "doc"));
        CHECK(!strcmp(argp_option.arg, "arg"));
        CHECK(argp_option.flags == to_int(of::arg_optional));
        CHECK(argp_option.group == 123);
    }
}

}
