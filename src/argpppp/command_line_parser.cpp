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
    parser_context() {}
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
    parser_context context;
    result.errnum = argp_parse(&argp, argc, argv, to_uint(m_flags), nullptr, &context);

    return result;
}

error_t command_line_parser::parse_option_static(int /*key*/, char* /*arg*/, argp_state* state)
{
    // TODO: real implementation
    //       * delegate to non-static version, return that one's exit code
    //       * catch all exceptions and store them in context, so they can be rethrown later. In the case of an exception, return EINVAL;
    auto context = get_context(state);
    return 0;
}

}
