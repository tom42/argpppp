// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <optional>
#include <stdexcept>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::of;
using argpppp::option;
using argpppp::optional_string;
using std::nullopt;

TEST_CASE("option")
{
    SECTION("constructor, all arguments specified")
    {
        const option opt('n', "name", "doc", "arg", of::arg_optional, 123);
        CHECK(opt.key() == 'n');
        CHECK(opt.name() == "name");
        CHECK(opt.doc() == "doc");
        CHECK(opt.arg() == "arg");
        CHECK(opt.flags() == of::arg_optional);
        CHECK(opt.group() == 123);
    }

    SECTION("constructor, all arguments use default values")
    {
        const option opt;
        CHECK(opt.key() == 0);
        CHECK(opt.name() == nullopt);
        CHECK(opt.doc() == nullopt);
        CHECK(opt.arg() == nullopt);
        CHECK(opt.flags() == of::none);
        CHECK(opt.group() == 0);
    }

    SECTION("constructor throws if a long name is required but not given")
    {
        CHECK(option('x', {}).name().has_value() == false);
        CHECK_THROWS_MATCHES(
            option(1, {}),
            std::invalid_argument,
            Catch::Matchers::Message("option requires a long name"));
    }

    SECTION("get_names")
    {
        CHECK(get_names(option('s', {})) == "-s");
        CHECK(get_names(option(1, "long-name")) == "--long-name");
        CHECK(get_names(option('s', "long-name")) == "--long-name (-s)");
        CHECK_THROWS_MATCHES(
            get_names(option()),
            std::invalid_argument,
            Catch::Matchers::Message("option has no name"));
    }

    SECTION("get_error_message")
    {
        const option switch_opt('s');
        const option opt_with_optional_argument('o', {}, {}, "OPTIONAL", of::arg_optional);
        const option opt_with_mandatory_argument('m', {}, {}, "MANDATORY");

        CHECK(get_error_message(switch_opt, "argument is ignored for switches") == "unexpected option '-s'");
        CHECK(get_error_message(switch_opt, nullptr) == "unexpected option '-s'");

        CHECK(get_error_message(opt_with_optional_argument, "badarg") == "invalid argument 'badarg' for option '-o'");
        CHECK(get_error_message(opt_with_optional_argument, nullptr) == "unexpected option '-o'");

        CHECK(get_error_message(opt_with_mandatory_argument, "badarg") == "invalid argument 'badarg' for option '-m'");
        CHECK(get_error_message(opt_with_mandatory_argument, nullptr) == "invalid argument '(null)' for option '-m'"); // argp_parse should not let this ever happen
    }
}

}
