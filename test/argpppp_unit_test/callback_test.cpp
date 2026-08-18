// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::callback;
using argpppp::error;
using argpppp::ok;
using argpppp::option;
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
}

}
