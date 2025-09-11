// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

export module argpppp:command_line_parser;
import :options;

namespace argpppp
{

export void parse_command_line(int argc, char* argv[], const options& options);

// TODO: flesh out public interface. This gets called by the convenience function above, but can also be called by programs that need more control
export class command_line_parser final
{
public:
    // TODO: this needs a return value
    void parse(int argc, char* argv[], const options& o) const;

private:
};

}
