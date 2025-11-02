// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <argp.h>
#include <functional>
#include <string>
#include <utility>
#include <vector>

export module argpppp:command_line_parser;
import :options;
import :pf;

namespace argpppp
{

export class failure final
{
public:
    failure(int status, int errnum, std::string message)
        : m_status(status)
        , m_errnum(errnum)
        , m_message(std::move(message))
    {}

    // TODO: add getters? (currently nobody is even using this...)

    bool operator==(const failure&) const = default;

private:
    int m_status;
    int m_errnum;
    std::string m_message;
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
