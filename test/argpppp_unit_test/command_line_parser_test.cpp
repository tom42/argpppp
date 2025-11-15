// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <cstdlib>
#include <cstring>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

import argpppp;

namespace argpppp_unit_test
{

using callback = argpppp::callback;
using command_line_parser = argpppp::command_line_parser;
using option_handler_result = argpppp::option_handler_result;
using parse_result = argpppp::parse_result;
using pf = argpppp::pf;
using runtime_error = std::runtime_error;
using argpppp::error;
using argpppp::failure;
using argpppp::ok;
using argpppp::value;
using std::string;
using std::vector;

namespace
{

template <typename Iterator>
vector<char> make_arg(Iterator begin, Iterator end)
{
    vector<char> arg(begin, end);
    arg.push_back(0); // Add terminating zero
    return arg;
}

vector<char> make_arg(const char* s)
{
    return make_arg(s, s + strlen(s));
}

class command_line_parser_fixture
{
public:
    command_line_parser_fixture()
    {
        parser
            .flags(pf::no_errs | pf::no_exit)
            .failure_callback([this](failure f) { failures.push_back(f); });
    }

protected:
    parse_result parse_command_line(const string& command_line)
    {
        // Build vector of zero terminated arguments
        // Split at space characters without quoting, arguments containing spaces are therefore not supported.
        // This is sufficient for our test cases, quoting and such are low level details handled by getopt.
        vector<vector<char>> args;
        args.push_back(make_arg("program_name"));
        for (auto word : std::views::split(command_line, ' '))
        {
            args.push_back(make_arg(word.begin(), word.end()));
        }

        // Build argv, a vector containing char pointers to the zero terminated arguments
        vector<char*> argv;
        for (auto& arg : args)
        {
            argv.push_back(arg.data());
        }

        return parser.parse(static_cast<int>(argv.size()), argv.data(), options);
    }

    command_line_parser parser;
    argpppp::options options;
    vector<failure> failures;
};

}

TEST_CASE_METHOD(command_line_parser_fixture, "command_line_parser")
{
    SECTION("Unlimited number of arguments")
    {
        auto result = parse_command_line("arg1 arg2 arg3 arg4");

        CHECK(result.errnum == 0);
        CHECK(result.args == vector<string>{"arg1", "arg2", "arg3", "arg4"});
        CHECK(failures.empty());
    }

    SECTION("Too few arguments")
    {
        options.num_args(2);

        auto result = parse_command_line("x");

        CHECK(result.errnum == EINVAL);
        CHECK(result.args == vector<string>{"x"});
        CHECK(failures == vector<failure>{ failure(EXIT_FAILURE, 0, "too few arguments") });
    }

    SECTION("Too many arguments")
    {
        options.num_args(2);

        auto result = parse_command_line("x y z");

        CHECK(result.errnum == EINVAL);
        CHECK(result.args == vector<string>{"x", "y"});
        CHECK(failures == vector<failure>{ failure(EXIT_FAILURE, 0, "too many arguments") });
    }

    SECTION("Correct number of arguments")
    {
        options.num_args(2);

        auto result = parse_command_line("x y");

        CHECK(result.errnum == 0);
        CHECK(result.args == vector<string>{"x", "y"});
        CHECK(failures.empty());
    }

    SECTION("Minimum and maximum number of arguments differ")
    {
        options.min_args(2);
        options.max_args(3);

        CHECK(parse_command_line("1").errnum == EINVAL);
        CHECK(parse_command_line("1 2").errnum == 0);
        CHECK(parse_command_line("1 2 3").errnum == 0);
        CHECK(parse_command_line("1 2 3 4").errnum == EINVAL);
    }

    SECTION("Exceptions abort parsing and are propagated to caller")
    {
        options
            .add({ 'a' }, callback([](auto, auto) -> option_handler_result { throw runtime_error("This exception should occur."); }))
            .add({ 'b' }, callback([](auto, auto) -> option_handler_result { throw runtime_error("This exception should not occur."); }));

        CHECK_THROWS_MATCHES(
            parse_command_line("-a -b"),
            runtime_error,
            Catch::Matchers::Message("This exception should occur."));
    }

    SECTION("Successful parsing of switches using callback option handlers")
    {
        bool a_seen = false;
        bool b_seen = false;
        bool c_seen = false;

        options
            .add({ 'a' }, callback([&](auto, auto) { a_seen = true; return ok(); }))
            .add({ 'b' }, callback([&](auto, auto) { b_seen = true; return ok(); }))
            .add({ 'c' }, callback([&](auto, auto) { c_seen = true; return ok(); }));

        auto result = parse_command_line("-c -a");

        CHECK(result.errnum == 0);
        CHECK(failures.empty());
        CHECK(a_seen == true);
        CHECK(b_seen == false);
        CHECK(c_seen == true);
    }

    SECTION("Parsing should stop if an option handler returns an error")
    {
        bool a_seen = false;

        options
            .add({ 'a' }, callback([&](auto, auto) -> option_handler_result { a_seen = true; return error("error message"); }))
            .add({ 'b' }, callback([](auto, auto) -> option_handler_result { throw runtime_error("This exception should not occur."); }));

        auto result = parse_command_line("-a -b");

        CHECK(result.errnum == EINVAL);
        CHECK(failures == vector<failure>{ failure(EXIT_FAILURE, 0, "error message") });
        CHECK(a_seen == true);
    }

    SECTION("Parsing of values")
    {
        int i = 0;
        int j = 0;
        string s = "";

        options
            .add({ 'i', {}, {}, "NUMBER" }, value(i))
            .add({ 'j', {}, {}, "NUMBER" }, value(j))
            .add({ 's', {}, {}, "STRING" }, value(s));

        auto result = parse_command_line("-i 123 -j -456 -s foo");

        CHECK(result.errnum == 0);
        CHECK(failures.empty());
        CHECK(i == 123);
        CHECK(j == -456);
        CHECK(s == "foo");
    }

    SECTION("Options without short names whose keys are automatically assigned")
    {
        int value1 = 0;
        int value2 = 0;

        options
            .add({ {}, "option1", {}, "N" }, value(value1))
            .add({ {}, "option2", {}, "N" }, value(value2));

        auto result = parse_command_line("--option1=1 --option2=2");

        CHECK(result.errnum == 0);
        CHECK(failures.empty());
        CHECK(value1 == 1);
        CHECK(value2 == 2);
    }
}

}
