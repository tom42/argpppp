// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <argp.h>
#include <cstdlib>
#include <exception>
#include <map>
#include <memory>
#include <string>

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
        , option_handlers(get_option_handlers(o))
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
    std::map<int, std::shared_ptr<option_handler>> option_handlers;
};

parser_context* get_context(argp_state* state) noexcept
{
    return static_cast<parser_context*>(state->input);
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
    auto context = get_context(state);
    try
    {
        return context->this_parser.parse_option(key, arg, state);
    }
    catch (...)
    {
        // Do not let exception escape into argp, which is written in C.
        // Instead, pass exception to calling C++ code through argpppp_context instance.
        context->exception = std::current_exception();
        return EINVAL;
    }
}

error_t command_line_parser::parse_option(int key, char* arg, argp_state* state) const
{
    auto context = get_context(state);

    auto handler = context->option_handlers.find(key);
    if (handler != context->option_handlers.end())
    {
        const auto handler_result = handler->second->handle_option(arg);
        return handle_option_handler_result(handler_result, state);
    }

    switch (key)
    {
        case ARGP_KEY_ARG:
            return handle_key_arg(arg, state);
        case ARGP_KEY_END:
            return handle_key_end(state);
        default:
            return ARGP_ERR_UNKNOWN;
    }
}

error_t command_line_parser::handle_key_arg(char* arg, argp_state* state) const
{
    auto context = get_context(state);

    if (context->result.args.size() >= context->opts.max_args())
    {
        report_failure(state, EXIT_FAILURE, 0, "too many arguments");
        return EINVAL;
    }

    context->result.args.push_back(arg);
    return 0;
}

error_t command_line_parser::handle_key_end(argp_state* state) const
{
    auto context = get_context(state);

    if (context->result.args.size() < context->opts.min_args())
    {
        report_failure(state, EXIT_FAILURE, 0, "too few arguments");
        return EINVAL;
    }

    return 0;
}

error_t command_line_parser::handle_option_handler_result(const option_handler_result& result, argp_state* state) const
{
    if (!result.is_success())
    {
        // TODO: actually we could delegate default error message creation to ARGP (by passing nonzero for errnum, I am just not sure how much sense this makes
        //       * Well argp_failure always prints error message (it pretty much expects it to be a non-empty string), followed by the error message for error_number
        //         if error_number is not zero, so this can only be used to supply additional info (of limited usefulness to be honest)
        //       * We must also think about doing this in a sane way:
        //         * If error_number is nonzero, should it then not also be the return value of this function and of argp_parse itself?
        //         * But what if we want an Unixoid return value but not print it?
        //           * We'd have to have another flag for this
        //           * => If we're not willing to implement all of this then we're probably better off not providing it on option_handler_result?
        // TODO: get all of this from resultr object
        //       * exit status (EXIT_FAILURE by default)
        //       * errnum (0 by default, anything else will cause output) => do we document this?
        report_failure(state, result.exit_status(), result.error_number(), result.error_message());
        return EINVAL;
    }

    return 0;
}

// TODO: remove?
void command_line_parser::report_option_error(int key, char* arg, argp_state* state, const char* additional_info) const
{
    auto option = find_option_or_throw(get_context(state)->opts, key);
    auto error_message = get_error_message(option, arg, additional_info);
    report_failure(state, EXIT_FAILURE, 0, error_message);
}

void command_line_parser::report_failure(const argp_state* state, int status, int errnum, const std::string& message) const
{
    if (m_failure_callback)
    {
        // TODO: should this also get exitcode? should we even have this callback on the public interface for client code? Or should we only have it as a test hook?
        m_failure_callback(errnum, message);
    }
    else
    {
        argp_failure(state, status, errnum, "%s", message.c_str());
    }
}

}
