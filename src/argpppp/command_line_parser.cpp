// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <argp.h>
#include <cassert>
#include <cstdlib>
#include <exception>
#include <memory>
#include <string>
#include <utility>

module argpppp;

namespace argpppp
{

namespace
{

class parser_context final
{
public:
    parser_context(const parser_context&) = delete;
    parser_context& operator=(const parser_context&) = delete;

    parser_context(const options& o, const command_line_parser& p, parse_result& r)
        : opts(o)
        , this_parser(p)
        , result(r)
    {}

    void rethrow_exception_if_any() const
    {
        if (exception)
        {
            std::rethrow_exception(exception);
        }
    }

    const options& opts;
    const command_line_parser& this_parser;
    parse_result& result;
    std::exception_ptr exception;
};

parser_context& get_context(argp_state* state) noexcept
{
    return *static_cast<parser_context*>(state->input);
}

}

parse_result parse_command_line(int argc, char* argv[], const options& options)
{
    command_line_parser parser;
    return parser.parse(argc, argv, options);
}

parse_result command_line_parser::parse(int argc, char* argv[], const options& options) const
{
    constexpr const argp_child* children = nullptr;
    constexpr const auto help_filter = nullptr;
    constexpr const char* argp_domain = nullptr;
    const auto argp_options = get_argp_options(options);

    const argp argp
    {
        argp_options.data(),
        parse_option_static,
        c_str(options.args_doc()),
        c_str(options.doc()),
        children,
        help_filter,
        argp_domain
    };

    parse_result result;
    parser_context context(options, *this, result);
    result.errnum = argp_parse(&argp, argc, argv, to_uint(m_flags), nullptr, &context);

    context.rethrow_exception_if_any();
    return result;
}

error_t command_line_parser::parse_option_static(int key, char* arg, argp_state* state)
{
    parser_context& context = get_context(state);
    try
    {
        return context.this_parser.parse_option(key, arg, state);
    }
    catch (...)
    {
        // Do not let exception escape into argp, which is written in C.
        // Instead, pass exception to calling C++ code through argpppp_context instance.
        context.exception = std::current_exception();
        return EINVAL;
    }
}

error_t command_line_parser::parse_option(int key, char* arg, argp_state* state) const
{
    switch (key)
    {
        case ARGP_KEY_ARG:
            return handle_key_arg(arg, state);
        case ARGP_KEY_END:
            return handle_key_end(state);
        default:
            parser_context& context = get_context(state);
            const option_with_handler* owh = context.opts.find_option(key);
            if (owh != nullptr)
            {
                const auto handler_result = owh->handle_option(arg);
                return handle_option_handler_result(handler_result, state);
            }
            return ARGP_ERR_UNKNOWN;
    }
}

error_t command_line_parser::handle_key_arg(char* arg, argp_state* state) const
{
    parser_context& context = get_context(state);

    if (context.result.args.size() >= context.opts.max_args())
    {
        report_failure(state, EXIT_FAILURE, 0, "too many arguments");
        return EINVAL;
    }

    context.result.args.push_back(arg);
    return 0;
}

error_t command_line_parser::handle_key_end(argp_state* state) const
{
    parser_context& context = get_context(state);

    if (context.result.args.size() < context.opts.min_args())
    {
        report_failure(state, EXIT_FAILURE, 0, "too few arguments");
        return EINVAL;
    }

    return 0;
}

error_t command_line_parser::handle_option_handler_result(const option_handler_result& result, argp_state* state) const
{
    if (result.is_success())
    {
        assert(result.error_number() == 0);
        return 0;
    }
    else
    {
        assert(result.error_number() != 0);
        int reported_error_number = result.include_standard_error_message() ? result.error_number() : 0;
        report_failure(state, result.exit_status(), reported_error_number, result.error_message().c_str());
        return result.error_number();
    }
}

void command_line_parser::report_failure(const argp_state* state, int status, int errnum, const char* message) const
{
    if (m_failure_callback)
    {
        // TODO: should this also get exitcode? should we even have this callback on the public interface for client code? Or should we only have it as a test hook?
        //       well...two things:
        //       * consider not exposing it
        //       * see whether we can pass it an option_handler_result (then again, this has nothing to do with that...)
        m_failure_callback(failure(status, errnum, message));
    }
    else
    {
        argp_failure(state, status, errnum, "%s", message);
    }
}

}
