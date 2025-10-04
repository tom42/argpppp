// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <string>
#include <utility>

export module argpppp:option_handler_result;
import :option;

namespace argpppp
{

export class option_handler_result final
{
public:
    static option_handler_result ok()
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

    bool include_standard_error_message() const
    {
        // We might make this configurable at some point, but for the time being it is just here as an idea.
        return false;
    }

    bool operator==(const option_handler_result&) const = default;

private:
    option_handler_result(bool is_success, int exit_status, int error_number, std::string error_message)
        : m_is_success(is_success)
        , m_exit_status(exit_status)
        , m_error_number(error_number)
        , m_error_message(std::move(error_message))
    {}

    bool m_is_success;
    int m_exit_status;
    int m_error_number;
    std::string m_error_message;
};

export option_handler_result ok();

export option_handler_result error(std::string message);

export option_handler_result error(const option& option, const char* arg, const std::string& message);

export option_handler_result error(const option& option, const char* arg, const char* message);

}
