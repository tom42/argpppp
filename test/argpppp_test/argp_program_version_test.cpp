// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

import argpppp;

// extern "C" is needed for some platforms, e.g. when using MSVC and argp-standalone.
// It is not needed for glibc.
extern "C"
{
const char* argp_program_version = "argp_program_version_test 1.0";
}

int main(int argc, char** argv)
{
    argpppp::options options;
    argpppp::command_line_parser parser;

    parser.parse(argc, argv, options);
}
