// SPDX - FileCopyrightText: 2025 Thomas Mathys
// SPDX - License - Identifier: MIT

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

import argpppp;

// TODO: set up program version  thing

int main(int argc, char* argv[])
{
    using argpppp::value;
    using string = std::string;

    try
    {
        int i = 0;
        string s = "";

        // TODO: show args_doc() feature
        // TODO: show bug report address feature
        argpppp::options options;
        options
            .doc("argpppp example program - Copyright (C) 2025 Thomas Mathys")
            .add_header("General options")
            .add({ 'i', "integer", "Option with mandatory integer argument", "INTEGER" }, value(i))
            .add({ 's', "string", "Option with mandatory string argument", "STRING" }, value(s))
            .add_header("Special options");

        // Parse command line.
        // Note that in the case of an error, argp_parse will print error output to stderr and exit with an error code.
        // Also note that if a help option is encountered, argp_parse will also print output and exit with success.
        auto parse_result = argpppp::parse_command_line(argc, argv, options);
        if (!parse_result.success())
        {
            return EXIT_FAILURE;
        }

        std::cout << "i: " << i << "\n";
        std::cout << "s: <" << s << ">\n";

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << argv[0] << ": " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
