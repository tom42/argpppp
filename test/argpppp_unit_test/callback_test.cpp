// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::error;

TEST_CASE("callback")
{
    argpppp::option opt('s', {}, {}, "STRING");
    argpppp::callback callback([](const argpppp::option& o, const char* a) { return error(o, a, "horrible error"); });

    auto result = callback.handle_option(opt, "argh!");

    CHECK(result == error("invalid argument 'argh!' for option '-s': horrible error"));
}

}
