// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

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
    pf m_flags = pf::none;
};

}
