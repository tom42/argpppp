// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <argp.h>
#include <functional>
#include <string>
#include <utility>
#include <vector>

export module argpppp:command_line_parser;
import :option_handlers;
import :options;
import :pf;

namespace argpppp
{

export struct failure final
{
    failure(int status, int errnum, std::string message)
        : status(status)
        , errnum(errnum)
        , message(std::move(message))
    {}

    bool operator==(const failure&) const = default;

    int status;
    int errnum;
    std::string message;
};

export using failure_callback = std::function<void(failure f)>;

export struct parse_result final
{
    bool success() const { return errnum == 0; }

    int errnum = 0;
    std::vector<std::string> args;
};

export parse_result parse_command_line(int argc, char* argv[], const options& options);

export class command_line_parser final
{
public:
    parse_result parse(int argc, char* argv[], const options& o) const;

    command_line_parser& failure_callback(const failure_callback& c)
    {
        m_failure_callback = c;
        return *this;
    }

    command_line_parser& flags(pf flags)
    {
        m_flags = flags;
        return *this;
    }

private:
    static error_t parse_option_static(int key, char* arg, argp_state* state);

    error_t parse_option(int key, char* arg, argp_state* state) const;

    error_t handle_key_arg(char* arg, argp_state* state) const;

    error_t handle_key_end(argp_state* state) const;

    error_t handle_option_handler_result(const option_handler_result& result, argp_state* state) const;

    void report_failure(const argp_state* state, int status, int errnum, const char* message) const;

    argpppp::failure_callback m_failure_callback;
    pf m_flags = pf::none;
};

}
