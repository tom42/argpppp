// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <limits>
#include <memory>

import argpppp;

namespace argpppp_unit_test
{

using callback = argpppp::callback;
using of = argpppp::of;
using option_handler = argpppp::option_handler;
using option_handler_result = argpppp::option_handler_result;
using options = argpppp::options;

namespace
{

class null_option_handler final : public option_handler
{
public:
    option_handler_result handle_option() override { return true; }
};

}

TEST_CASE("options_test")
{
    options options;

    SECTION("state after construction")
    {
        CHECK(options.doc().has_value() == false);
        CHECK(options.args_doc().has_value() == false);
        CHECK(options.min_args() == 0);
        CHECK(options.max_args() == std::numeric_limits<std::size_t>::max());
    }

    SECTION("set program documentation")
    {
        options.doc("Program documentation");
        CHECK(options.doc() == "Program documentation");
    }

    SECTION("set argument documentation")
    {
        options.args_doc("Argument documentation");
        CHECK(options.args_doc() == "Argument documentation");
    }

    SECTION("set minimum number of arguments")
    {
        options.min_args(1);
        CHECK(options.min_args() == 1);
    }

    SECTION("set maximum number of arguments")
    {
        options.max_args(10);
        CHECK(options.max_args() == 10);
    }

    SECTION("set number of arguments")
    {
        options.num_args(2);
        CHECK(options.max_args() == 2);
        CHECK(options.max_args() == 2);
    }

    SECTION("add throws if an option with key = 0 has a handler")
    {
        CHECK_THROWS_MATCHES(
            options.add({ {}, "This is a documentation option", {}, {}, of::doc }, std::make_shared<null_option_handler>()),
            std::invalid_argument,
            Catch::Matchers::Message("add: a special option with key = 0 must not have a handler"));
    }

    SECTION("add throws if an option with key != 0 does not have a handler")
    {
        CHECK_THROWS_MATCHES(
            options.add({ 'a' }, {}),
            std::invalid_argument,
            Catch::Matchers::Message("add: option must have a handler"));
    }
}

}
