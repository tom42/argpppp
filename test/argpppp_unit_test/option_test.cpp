// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <limits>
#include <optional>
#include <vector>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::is_printable_key;
using argpppp::need_long_name;
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
            option(256, {}),
            std::invalid_argument,
            Catch::Matchers::Message("option without printable short name needs a long name"));
    }

    SECTION("is_printable_key")
    {
        CHECK(is_printable_key(std::numeric_limits<int>::min()) == false);
        CHECK(is_printable_key(-1) == false);
        CHECK(is_printable_key(0) == false);
        CHECK(is_printable_key(31) == false);
        CHECK(is_printable_key(32) == true);
        CHECK(is_printable_key(126) == true);
        CHECK(is_printable_key(127) == false);
        CHECK(is_printable_key(std::numeric_limits<int>::max()) == false);
    }

    SECTION("need_long_name")
    {
        CHECK(need_long_name(0) == false);
        CHECK(need_long_name(1) == true);
        CHECK(need_long_name('a') == false);
    }

    SECTION("get_names")
    {
        CHECK(get_names(option('s', {})) == "-s");
        CHECK(get_names(option({}, "long-name")) == "--long-name");
        CHECK(get_names(option('s', "long-name")) == "--long-name (-s)");
        CHECK_THROWS_MATCHES(
            get_names(option()),
            std::invalid_argument,
            Catch::Matchers::Message("get_names: option has no name"));
    }

    SECTION("get_error_message")
    {
        const option switch_option('s');
        const option option_with_optional_argument('o', {}, {}, "OPTIONAL", of::arg_optional);
        const option option_with_mandatory_argument('m', {}, {}, "MANDATORY");

        CHECK(get_error_message(switch_option, "argument is ignored for switches") == "unexpected option '-s'");
        CHECK(get_error_message(switch_option, nullptr) == "unexpected option '-s'");

        CHECK(get_error_message(option_with_optional_argument, "badarg") == "invalid argument 'badarg' for option '-o'");
        CHECK(get_error_message(option_with_optional_argument, nullptr) == "unexpected option '-o'");

        CHECK(get_error_message(option_with_mandatory_argument, "badarg") == "invalid argument 'badarg' for option '-m'");
        CHECK(get_error_message(option_with_mandatory_argument, nullptr) == "invalid argument '(null)' for option '-m'"); // argp_parse should not let this ever happen
    }

    SECTION("to_argp_option")
    {
        const option opt('n', "name", "doc", "arg", of::arg_optional, 123);
        const auto argp_option = to_argp_option(opt);

        CHECK(argp_option.key == 'n');
        CHECK(!strcmp(argp_option.name, "name"));
        CHECK(!strcmp(argp_option.doc, "doc"));
        CHECK(!strcmp(argp_option.arg, "arg"));
        CHECK(argp_option.flags == to_int(of::arg_optional));
        CHECK(argp_option.group == 123);
    }
}

}
