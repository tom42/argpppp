// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <argp.h>

export module argpppp:command_line_parser;
import :options;
import :pf;

namespace argpppp
{

// TODO: add argument vector
export struct parse_result final
{
    int errnum = 0;
};

export parse_result parse_command_line(int argc, char* argv[], const options& options);

export class command_line_parser final
{
public:
    parse_result parse(int argc, char* argv[], const options& o) const;

    void flags(pf flags)
    {
        m_flags = flags;
    }

private:
    static error_t parse_option_static(int key, char* arg, argp_state* state);

    error_t parse_option(int key, char* arg, argp_state* state) const;

    pf m_flags = pf::none;
};

}
