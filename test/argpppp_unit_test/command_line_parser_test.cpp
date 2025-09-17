// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
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
using options = argpppp::options;
using parse_result = argpppp::parse_result;
using pf = argpppp::pf;
using runtime_error = std::runtime_error;
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
            .failure_callback([this](int, const string& message) {failure_message += message; });
    }

protected:
    parse_result parse_command_line(const string& command_line)
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

        return parser.parse(static_cast<int>(argv.size()), argv.data(), options);
    }

    command_line_parser parser;
    options options;
    string failure_message;
};

}

TEST_CASE_METHOD(command_line_parser_fixture, "command_line_parser_test")
{
    SECTION("Unlimited number of arguments")
    {
        auto result = parse_command_line("arg1 arg2 arg3 arg4");

        CHECK(result.errnum == 0);
        CHECK(result.args == vector<string>{"arg1", "arg2", "arg3", "arg4"});
        CHECK(failure_message == "");
    }

    SECTION("Too few arguments")
    {
        options.num_args(2);

        auto result = parse_command_line("x");

        CHECK(result.errnum == EINVAL);
        CHECK(result.args == vector<string>{"x"});
        CHECK(failure_message == "too few arguments");
    }

    SECTION("Too many arguments")
    {
        options.num_args(2);

        auto result = parse_command_line("x y z");

        CHECK(result.errnum == EINVAL);
        CHECK(result.args == vector<string>{"x", "y"});
        CHECK(failure_message == "too many arguments");
    }

    SECTION("Correct number of arguments")
    {
        options.num_args(2);

        auto result = parse_command_line("x y");

        CHECK(result.errnum == 0);
        CHECK(result.args == vector<string>{"x", "y"});
        CHECK(failure_message == "");
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
            .add({ 'a' }, callback([]{ throw runtime_error("This exception should occur."); }))
            .add({ 'b' }, callback([]{ throw runtime_error("This exception should not occur."); }));

        CHECK_THROWS_MATCHES(
            parse_command_line("-a -b"),
            runtime_error,
            Catch::Matchers::Message("This exception should occur."));
    }
}

}
