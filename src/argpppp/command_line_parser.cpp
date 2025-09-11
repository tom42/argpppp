// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module argpppp;

namespace argpppp
{

void parse_command_line(int argc, char* argv[], const options& options)
{
    command_line_parser parser;
    return parser.parse(argc, argv, options);
}

}
