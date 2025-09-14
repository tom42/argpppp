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

    SECTION("Correct number of arguments")
    {
        options.num_args(2);

        auto result = parse_command_line(parser, options, "x y");

        // TODO: check return code, should be 0
        // TODO: check argument vector in result
        // TODO: check error output, should be empty
        // TODO: when done, delete old test from parser_test_old.cpp (after checking it is equivalent)
    }
}

}
