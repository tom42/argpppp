// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <cerrno>
#include <cstring>
#include <functional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

import argpppp;

namespace argpppp_unit_test
{

using argpppp_old::arg_error;
using argpppp::of;
using argpppp_old::parser;
using argpppp::pf;
using string = std::string;
using std::vector;

namespace
{

template <typename Iterator>
std::vector<char> make_arg(Iterator begin, Iterator end)
{
    std::vector<char> arg(begin, end);
    arg.push_back(0); // Add terminating zero
    return arg;
}

std::vector<char> make_arg(const char* s)
{
    return make_arg(s, s + strlen(s));
}

auto parse(parser& parser, const std::string& command_line)
{
    // 1) Build vector of zero terminated arguments.
    std::vector<std::vector<char>> args;
    args.push_back(make_arg("program_name"));
    for (auto word : std::views::split(command_line, ' '))
    {
        args.push_back(make_arg(word.begin(), word.end()));
    }

    // 2) Build argv, a vector containing char pointers to the zero terminated arguments
    std::vector<char*> argv;
    for (auto& arg : args)
    {
        argv.push_back(arg.data());
    }

    return parser.parse(static_cast<int>(argv.size()), argv.data(), pf::no_exit);
}

}

TEST_CASE("parser_test_old")
{
    parser parser;
    std::string failure_message;
    parser.set_failure_callback([&failure_message](int, const std::string& msg) { failure_message += msg; });

    SECTION("Parsing should stop if an option callback returns false")
    {
        bool a_seen = false;

        add_option(parser, { 'a' }, [&](auto){ a_seen = true; return false; });
        add_option(parser, { 'b' }, [](auto)->bool{ throw std::runtime_error("This exception should not occur."); });

        auto result = parse(parser, "-a -b");

        CHECK(result.errnum == EINVAL);
        CHECK(failure_message == "unexpected option '-a'");
        CHECK(a_seen == true);
    }

    SECTION("Parsing should stop if an option callback returns arg_error")
    {
        bool a_seen = false;

        add_option(parser, { 'a' }, [&](auto) { a_seen = true; return arg_error("custom error message"); });
        add_option(parser, { 'b' }, [](auto)->bool { throw std::runtime_error("This exception should not occur."); });

        auto result = parse(parser, "-a -b");

        CHECK(result.errnum == EINVAL);
        CHECK(failure_message == "custom error message");
        CHECK(a_seen == true);
    }

    SECTION("set_nargs throws if min is > max")
    {
        CHECK_THROWS_MATCHES(
            parser.set_nargs(3, 2),
            std::logic_error,
            Catch::Matchers::Message("min_args must be <= max_args"));
    }
}

}
