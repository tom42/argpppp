// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <argp.h>
#include <cstddef>
#include <functional>
#include <limits>
#include <map>
#include <string>
#include <variant>
#include <vector>

export module argpppp:parser;
import :of;
import :option;
import :optional_string;
import :pf;

namespace argpppp
{

export class arg_error final
{
public:
    explicit arg_error(const std::string& message) : m_message(message) {}

    const std::string& message() const { return m_message; }
private:
    std::string m_message;
};

export using option_callback_result = std::variant<bool, arg_error>;
export using option_callback = std::function<option_callback_result(char*)>;

export using failure_callback = std::function<void(int, const std::string&)>;

export struct parse_result final
{
    int errnum = 0;
    std::vector<std::string> args;
};

export class parser final
{
public:
    void add_option(const option& o, const option_callback& c);

    void set_args_doc(const optional_string& s);

    void set_doc(const optional_string& s);

    void set_failure_callback(const failure_callback& c);

    void set_nargs(std::size_t nargs);

    void set_nargs(std::size_t min_args, std::size_t max_args);

    parse_result parse(int argc, char** argv, pf flags = pf::none) const;

private:
    error_t parse_option(int key, char *arg, argp_state *state) const;
    static error_t parse_option_static(int key, char *arg, argp_state *state);

    error_t handle_option_callback_result(const option_callback_result& result, int key, char* arg, const argp_state* state) const;
    error_t handle_option_callback_result_for_type(bool result, int key, char* arg, const argp_state* state) const;
    error_t handle_option_callback_result_for_type(const arg_error& error, int key, char* arg, const argp_state* state) const;

    error_t handle_key_arg(char* arg, argp_state* state) const;
    error_t handle_key_end(argp_state* state) const;

    void report_failure(const argp_state* state, int status, int errnum, const std::string& message) const;

    optional_string m_args_doc;
    optional_string m_doc;
    std::vector<option> m_options;
    std::map<int, option_callback> m_callbacks;
    failure_callback m_failure_callback;

    // Workaround for MSVC x86: can't fully qualify std::size_t when used as a template argument below
    using size_t = std::size_t;
    std::size_t m_min_args = std::numeric_limits<size_t>::min();
    std::size_t m_max_args = std::numeric_limits<size_t>::max();
};

export inline void add_option(parser& p, const option& o, const option_callback& c)
{
    p.add_option(o, c);
}

export inline void add_header(parser& p, const std::string& s, int group = 0)
{
    p.add_option(option({}, 0, {}, of::none, s, group), {});
}

}
