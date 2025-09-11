// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <argp.h>

module argpppp;

namespace argpppp
{

void parse_command_line(int argc, char* argv[], const options& options)
{
    command_line_parser parser;
    return parser.parse(argc, argv, options);
}

void command_line_parser::parse(int /*argc*/, char* /*argv*/[], const options& /*options*/) const
{
    // TODO: set up stuff for argp_parse (see old implementation)
    // TODO: call argp_parse

    constexpr const argp_child* children = nullptr;
    constexpr const auto help_filter = nullptr;
    constexpr const char* argp_domain = nullptr;

    // TODO: supply all the commented out stuff
    const argp argp{ {}/*argp_options.data()*/, {}/*parse_option_static*/, {}/*c_str(m_args_doc)*/, {}/*c_str(m_doc)*/, children, help_filter, argp_domain};
}

}
