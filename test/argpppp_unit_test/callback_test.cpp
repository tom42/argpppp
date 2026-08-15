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

TEST_CASE("callback")
{
    SECTION("without option parameter")
    {
        option opt({}, "--switch");
        callback callback([&] { return error("error 1"); });

        auto result = callback.handle_option(opt, nullptr);

        CHECK(result == error("error 1"));
    }

    SECTION("with option parameter")
    {
        option opt('s', {}, {}, "STRING");
        // TODO: remove capture by reference, that is a temporary hack
        // TODO: do not pass opt, take data from parameter object
        // TODO: do not pass "argh!" literal here, take data from parameter object
        callback callback([&](auto&&) { return error(opt, "argh!", "error 2"); });

        auto result = callback.handle_option(opt, "argh!");

        CHECK(result == error("invalid argument 'argh!' for option '-s': error 2"));
    }
}

}
