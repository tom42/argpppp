// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

import argpppp;

// Some of argp_parse's features are controlled by the existence of certain global variables.
// These are not wrapped by argpppp, but can be used nevertheless.
//
// Note that extern "C" is needed for some platforms, e.g. when using MSVC and argp-standalone.
// It is not needed for glibc.
extern "C"
{

// Defining this variable is optional. If it is defined, the --version option is available.
// The option prints the content of argp_program_version and exits.
const char* argp_program_version = "argpppp example 0.99.1";

// Defining this variable is optional. If it is defined, the 'Report bugs to ...'
// text will appear at the end of the message printed by the --help option.
const char* argp_program_bug_address = "https://github.com/tom42/argpppp/issues";

}

int main(int argc, char* argv[])
{
    using argpppp::value;
    using string = std::string;

    try
    {
        int i = 0;
        string s = "";
        bool x = false;
        bool y = false;
        int j = 1;

        argpppp::options options;
        options
            .doc("argpppp example program - Copyright (C) 2025 Thomas Mathys")
            .args_doc("ARG1 ARG2 ... ARGN")
            .add_header("General options")
            .add({ 'i', "integer", "Option with mandatory integer argument", "INTEGER" }, value(i))
            .add({ 's', "string", "Option with mandatory string argument", "STRING" }, value(s))
            .add_header("Special options")
            .add({ 'x', {}, "A switch" }, value(x))
            .add({ 'y', {}, "Another switch" }, value(y))
            .add({ 'j', "another-integer", "Another otion with mandatory integer argument", "INTEGER" }, value(j).min(1).max(10));

        // Parse command line.
        // Note that in the case of an error, argp_parse will print error output to stderr and exit with an error code.
        // Also note that if a help option is encountered, argp_parse will also print output and exit with success.
        auto parse_result = argpppp::parse_command_line(argc, argv, options);
        if (!parse_result.success())
        {
            return EXIT_FAILURE;
        }

        std::cout << std::boolalpha;
        std::cout << "i: " << i << "\n";
        std::cout << "s: '" << s << "'\n";
        std::cout << "x: " << x << "\n";
        std::cout << "y: " << y << "\n";
        std::cout << "j: " << j << "\n";

        // Program arguments are stored in parse_result.args.
        // It is possible to specify the minimum/maximum/exact number of arguments
        // using options::min_args(), options::max_args() and options::num_args().
        std::cout << "arguments:";
        if (parse_result.args.size())
        {
            for (auto& arg : parse_result.args)
            {
                std::cout << " '" << arg << '\'';
            }
        }
        else
        {
            std::cout << " (none)\n";
        }

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << argv[0] << ": " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
