// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::callback;
using argpppp::error;
using argpppp::option;

TEST_CASE("callback")
{
    option opt('s', {}, {}, "STRING");

    SECTION("without option parameter")
    {
        CHECK(false);
    }

    SECTION("with option parameter")
    {
        callback callback([](const option& o, const char* a) { return error(o, a, "horrible error"); });

        auto result = callback.handle_option(opt, "argh!");

        CHECK(result == error("invalid argument 'argh!' for option '-s': horrible error"));
    }
}

}
