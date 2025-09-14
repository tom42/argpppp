// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <argp.h>

module argpppp;

namespace argpppp
{

namespace
{

class parser_context final
{
public:
    // TODO: delete copy constructor
    // TODO: delete assignment operator

    parser_context(const command_line_parser& p, parse_result& r)
        : this_parser(p)
        , result(r)
    {}

    const command_line_parser& this_parser;
    parse_result& result;
};

parser_context* get_context(argp_state* state)
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
    // TODO: set up stuff for argp_parse (see old implementation)
    // TODO: call argp_parse

    constexpr const argp_child* children = nullptr;
    constexpr const auto help_filter = nullptr;
    constexpr const char* argp_domain = nullptr;

    // TODO: supply all the commented out stuff
    const argp argp{ {}/*argp_options.data()*/, parse_option_static, c_str(options.args_doc()), c_str(options.doc()), children, help_filter, argp_domain};

    // TODO: rethrow any exceptions
    parse_result result;
    parser_context context(*this, result);
    result.errnum = argp_parse(&argp, argc, argv, to_uint(m_flags), nullptr, &context);

    return result;
}

error_t command_line_parser::parse_option_static(int key, char* arg, argp_state* state)
{
    // TODO: real implementation
    //       * catch all exceptions and store them in context, so they can be rethrown later. In the case of an exception, return EINVAL;
    auto context = get_context(state);
    return context->this_parser.parse_option(key, arg, state);
}

error_t command_line_parser::parse_option(int key, char* arg, argp_state* state) const
{
    switch (key)
    {
        case ARGP_KEY_ARG:
            // TODO: move this into own function returning 0, so we just have a return statement here
            get_context(state)->result.args.push_back(arg);
            return 0;
        // TODO: handle ARGP_KEY_END here
        default:
            return ARGP_ERR_UNKNOWN;
    }
}

}
