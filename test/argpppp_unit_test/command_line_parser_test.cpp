// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <ranges>
#include <string>
#include <vector>

import argpppp;

namespace argpppp_unit_test
{

using command_line_parser = argpppp::command_line_parser;
using options = argpppp::options;
using parse_result = argpppp::parse_result;
using pf = argpppp::pf;
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

// TODO: rename this to disambiguate?
// TODO: review (arguments and that)
parse_result parse_command_line(command_line_parser& parser, const options& options, const string& command_line)
{
    // Build vector of zero terminated arguments
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

    parser.flags(pf::no_errs | pf::no_exit);
    return parser.parse(static_cast<int>(argv.size()), argv.data(), options);
}

}

TEST_CASE("command_line_parser_test")
{
    command_line_parser parser;
    options options;
    string failure_message;

    parser.failure_callback([&failure_message](int, const string& message) {failure_message += message; });

    SECTION("Unlimited number of arguments")
    {
        auto result = parse_command_line(parser, options, "arg1 arg2 arg3 arg4");

        CHECK(result.errnum == 0);
        CHECK(result.args == vector<string>{"arg1", "arg2", "arg3", "arg4"});
        CHECK(failure_message == "");
    }

    SECTION("Too few arguments")
    {
        options.num_args(2);

        auto result = parse_command_line(parser, options, "x");

        CHECK(result.errnum == EINVAL);
        CHECK(result.args == vector<string>{"x"});
        CHECK(failure_message == "too few arguments");
    }

    SECTION("Too many arguments")
    {
        options.num_args(2);

        auto result = parse_command_line(parser, options, "x y z");

        CHECK(result.errnum == EINVAL);
        CHECK(result.args == vector<string>{"x", "y"});
        CHECK(failure_message == "too many arguments");
    }

    SECTION("Correct number of arguments")
    {
        options.num_args(2);

        auto result = parse_command_line(parser, options, "x y");

        CHECK(result.errnum == 0);
        CHECK(result.args == vector<string>{"x", "y"});
        CHECK(failure_message == "");
    }
}

}
