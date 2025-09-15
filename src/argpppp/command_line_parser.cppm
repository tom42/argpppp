// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <argp.h>
#include <functional>
#include <string>
#include <vector>

export module argpppp:command_line_parser;
import :options;
import :pf;

namespace argpppp
{

export using failure_callback = std::function<void(int, const std::string&)>;

// TODO: add is_successful() / success() function?
export struct parse_result final
{
    int errnum = 0;
    std::vector<std::string> args;
};

export parse_result parse_command_line(int argc, char* argv[], const options& options);

export class command_line_parser final
{
public:
    parse_result parse(int argc, char* argv[], const options& o) const;

    // TODO: fluent API?
    void failure_callback(const failure_callback& c)
    {
        m_failure_callback = c;
    }

    // TODO: fluent API?
    void flags(pf flags)
    {
        m_flags = flags;
    }

private:
    static error_t parse_option_static(int key, char* arg, argp_state* state);

    error_t parse_option(int key, char* arg, argp_state* state) const;

    error_t handle_key_arg(char* arg, argp_state* state) const;

    error_t handle_key_end(argp_state* state) const;

    void report_failure(const argp_state* state, int status, int errnum, const std::string& message) const;

    argpppp::failure_callback m_failure_callback;
    pf m_flags = pf::none;
};

}
