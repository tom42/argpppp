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
        bool callback_executed = false;
        option opt({}, "--switch");
        callback callback([&] { callback_executed = true; return error("return value"); });

        auto result = callback.handle_option(opt, nullptr);

        CHECK(callback_executed);
        CHECK(result == ok());
    }

    SECTION("with option parameter")
    {
        option opt('s', {}, {}, "STRING");
        callback callback([](const option& o, const char* a) { return error(o, a, "horrible error"); });

        auto result = callback.handle_option(opt, "argh!");

        CHECK(result == error("invalid argument 'argh!' for option '-s': horrible error"));
    }
}

}
