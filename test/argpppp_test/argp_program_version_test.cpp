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
    argpppp::parser parser;

    // Work around undefined reference when doing debug builds with g++
    // (undefined reference to `std::_Optional_base<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, false, false>::_Optional_base())
    parser.add_option({}, {});

    parser.parse(argc, argv);
}
