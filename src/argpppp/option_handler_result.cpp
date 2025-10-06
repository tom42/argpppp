// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <format>
#include <string>

module argpppp;
import :option;

namespace argpppp
{

option_handler_result option_handler_result::ok()
{
    return option_handler_result(true, {}, {}, {});
}

option_handler_result option_handler_result::error(std::string error_message)
{
    return option_handler_result(false, EXIT_FAILURE, EINVAL, std::move(error_message));
}

option_handler_result ok()
{
    return option_handler_result::ok();
}

option_handler_result error(std::string message)
{
    return option_handler_result::error(std::move(message));
}

option_handler_result error(const option& option, const char* arg, const std::string& message)
{
    return error(option, arg, message.c_str());
}

option_handler_result error(const option& option, const char* arg, const char* message)
{
    return error(std::format("{}: {}", get_error_message(option, arg), message));
}

option_handler_result::option_handler_result(bool is_success, int exit_status, int error_number, std::string error_message)
    : m_is_success(is_success)
    , m_exit_status(exit_status)
    , m_error_number(error_number)
    , m_error_message(std::move(error_message))
{}

}
