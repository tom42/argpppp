// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <utility>
#include <variant>

import argpppp;

namespace argpppp_unit_test
{

TEST_CASE("value<std::signed_integral>")
{
    int32_t target;
    argpppp::value<int32_t> value(target);

    SECTION("successful parsing with default settings")
    {
        CHECK(std::get<bool>(value.handle_option("-2147483648")) == true);
        CHECK(target == -2147483648);

        CHECK(std::get<bool>(value.handle_option("2147483647")) == true);
        CHECK(target == 2147483647);
    }

    // TODO: before we continue here we should think about standard vs custom error messages:
    //       * How can we fit them together? Currently we have
    //         * For a switch:                 "unexpected option '-x'"
    //         * For an option with argument:  "invalid argument 'quux' for option '-z'"
    //       * As you can see, default error messages contain the option name.
    //       * Apparently our current implementation does not do that for custom error messages, but maybe it should?
    //         * The simplest way would be to do something like
    //           * option '-x': unexpected option
    //           * option '-z': invalid argument 'quux'
    //           * option '-y': bad moon phase, here's a custom error message for you
    //         * => Is probably the way to go, no?

    // TODO: test too small/too big => appropriate error message
    // TODO: test bad input => appropriate error message
    // TODO: test alternate base setting (e.g. 0 for autodetect and maybe some nonstandard base such as 6 or something)
}

}
