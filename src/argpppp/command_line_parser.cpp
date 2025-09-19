// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <argp.h>
#include <cstdlib>
#include <exception>
#include <map>
#include <memory>
#include <string>
#include <variant>

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
        : options(o)
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

    const options& options;
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
        return handle_option_handler_result(handler_result, key, arg, state);
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

    // TODO: recheck: is it > or is it >= ?
    if (context->result.args.size() >= context->options.max_args())
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

    if (context->result.args.size() < context->options.min_args())
    {
        report_failure(state, EXIT_FAILURE, 0, "too few arguments");
        return EINVAL;
    }

    return 0;
}

error_t command_line_parser::handle_option_handler_result(const option_handler_result& result, int key, char* arg, argp_state* state) const
{
    return std::visit([&](const auto& r) { return handle_option_handler_result_for_type(r, key, arg, state); }, result);
}

error_t command_line_parser::handle_option_handler_result_for_type(bool result, int key, char* arg, argp_state* state) const
{
    if (result)
    {
        return 0;
    }
    else
    {
        auto option = find_option_or_throw(get_context(state)->options, key);
        auto error_message = get_default_error_message(option, arg);
        report_failure(state, EXIT_FAILURE, 0, error_message);
        return EINVAL;
    }
}

error_t command_line_parser::handle_option_handler_result_for_type(const option_error& error, int, char*, argp_state* state) const
{
    report_failure(state, EXIT_FAILURE, 0, error.message());
    return EINVAL;
}

void command_line_parser::report_failure(const argp_state* state, int status, int errnum, const std::string& message) const
{
    if (m_failure_callback)
    {
        m_failure_callback(errnum, message);
    }
    else
    {
        argp_failure(state, status, errnum, "%s", message.c_str());
    }
}

}
