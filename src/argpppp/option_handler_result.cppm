// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <string>
#include <utility>

export module argpppp:option_handler_result;

namespace argpppp
{

// TODO: random comments I am not sure I still remember what I meant
//       * default message
//         * switch, option with optional argument when it is missing: "unexpected option '-x'"
//         * sonst: invalid argument 'bla' for option '-z'
// TODO: regarding the error code: I think we want to do this differently:
//       * We always do supply error_number and return that to argp_parse
//       * We can then have an additional flag telling whether we want to pass the error_number to argp_failure or not
export class option_handler_result final
{
public:
    static option_handler_result success()
    {
        return option_handler_result(true, {}, {}, {});
    }

    static option_handler_result error(std::string error_message)
    {
        return option_handler_result(false, EXIT_FAILURE, EINVAL, std::move(error_message));
    }

    bool is_success() const
    {
        return m_is_success != false;
    }

    int exit_status() const
    {
        return m_exit_status;
    }

    int error_number() const
    {
        return m_error_number;
    }

    const std::string& error_message() const
    {
        return m_error_message;
    }

    bool operator==(const option_handler_result&) const = default;

private:
    option_handler_result(bool is_success, int exit_status, int error_number, std::string error_message)
        : m_is_success(is_success)
        , m_exit_status(exit_status)
        , m_error_number(error_number)
        , m_error_message(std::move(error_message))
    {
    }

    bool m_is_success;
    int m_exit_status;
    int m_error_number;
    std::string m_error_message;
};

}
