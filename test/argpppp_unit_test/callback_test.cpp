// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <functional>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::callback;
using argpppp::error;
using argpppp::ok;
using argpppp::option;
using argpppp::option_handler_result;
using argpppp::option_occurrence;

TEST_CASE("callback")
{
    SECTION("without option parameter")
    {
        option opt({}, "--switch");
        callback callback([&] { return error("error 1"); });

        auto result = callback.handle_option(option_occurrence(opt, nullptr));

        CHECK(result == error("error 1"));
    }

    SECTION("with option parameter")
    {
        option opt('s', {}, {}, "STRING");
        callback callback([](auto&& o) { return error(o, "error 2"); });

        auto result = callback.handle_option(option_occurrence(opt, "argh!"));

        CHECK(result == error("invalid argument 'argh!' for option '-s': error 2"));
    }

    SECTION("no callback function passed to constructor")
    {
        callback(std::function<option_handler_result(void)>{}); // TODO: this should throw an exception
        callback(std::function<option_handler_result(option_occurrence)>{}); // TODO: this should throw the same exception
    }
}

}
