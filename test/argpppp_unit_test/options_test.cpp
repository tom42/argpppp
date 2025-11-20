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

using argpppp::callback;
using argpppp::of;
using argpppp::option;
using argpppp::option_handler;
using argpppp::option_handler_result;
using argpppp::options;
using argpppp::short_name;

namespace
{

class null_option_handler final : public option_handler
{
public:
    option_handler_result handle_option(const option&, const char*) const override
    {
        return argpppp::ok();
    }
};

}

TEST_CASE("options")
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
        // TODO: what are doc options, again, exactly? Are they allowed to have a nonzero key?
        CHECK_THROWS_MATCHES(
            options.add({ short_name::null(), "This is a documentation option", {}, {}, of::doc}, std::make_unique<null_option_handler>()),
            std::invalid_argument,
            Catch::Matchers::Message("a special option with key = 0 must not have a handler"));
    }

    SECTION("add throws if an option with key != 0 does not have a handler")
    {
        CHECK_THROWS_MATCHES(
            options.add({ 'a' }, {}),
            std::invalid_argument,
            Catch::Matchers::Message("option with key != 0 must have a handler"));
    }

    SECTION("add throws if an option with the same key already exists")
    {
        options.add({ 'a' }, std::make_unique<null_option_handler>());

        CHECK_THROWS_MATCHES(
            options.add({ 'a' }, std::make_unique<null_option_handler>()),
            std::invalid_argument,
            Catch::Matchers::Message("option with duplicate short name"));
    }

    SECTION("multiple options with key = 0 can be added")
    {
        options.add_header("header 1");
        options.add_header("header 2");
    }

    SECTION("add_header")
    {
        options.add_header("header text", 123);

        const auto argp_options = get_argp_options(options);

        CHECK(argp_options[0].key == 0);
        CHECK(argp_options[0].name == nullptr);
        CHECK(!strcmp(argp_options[0].doc, "header text"));
        CHECK(argp_options[0].arg == nullptr);
        CHECK(argp_options[0].flags == 0);
        CHECK(argp_options[0].group == 123);
    }

    SECTION("find_option")
    {
        options.add({ 'a' }, std::make_unique<null_option_handler>());
        options.add({ 'b' }, std::make_unique<null_option_handler>());

        CHECK(options.find_option('a')->opt().sname() == 'a');
        CHECK(options.find_option('b')->opt().sname() == 'b');
        CHECK(options.find_option('c') == nullptr);
    }

    SECTION("get_argp_options")
    {
        options
            .add({ 'o', "option1", "doc1", "arg1", of::arg_optional, 1 }, null_option_handler())
            .add({ 'p', "option2", "doc2", "arg2", of::no_usage, 2 }, null_option_handler());

        const auto argp_options = get_argp_options(options);

        CHECK(argp_options.size() == 3);

        CHECK(argp_options[0].key == 'o');
        CHECK(!strcmp(argp_options[0].name, "option1"));
        CHECK(!strcmp(argp_options[0].doc, "doc1"));
        CHECK(!strcmp(argp_options[0].arg, "arg1"));
        CHECK(argp_options[0].flags == to_int(of::arg_optional));
        CHECK(argp_options[0].group == 1);

        CHECK(argp_options[1].key == 'p');
        CHECK(!strcmp(argp_options[1].name, "option2"));
        CHECK(!strcmp(argp_options[1].doc, "doc2"));
        CHECK(!strcmp(argp_options[1].arg, "arg2"));
        CHECK(argp_options[1].flags == to_int(of::no_usage));
        CHECK(argp_options[1].group == 2);

        CHECK(argp_options[2].key == 0);
        CHECK(argp_options[2].name == nullptr);
        CHECK(argp_options[2].doc == nullptr);
        CHECK(argp_options[2].arg == nullptr);
        CHECK(argp_options[2].flags == 0);
        CHECK(argp_options[2].group == 0);
    }
}

}
