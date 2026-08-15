// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <functional>

export module argpppp:callback;
import :option;
import :option_handler;
import :option_handler_result;

namespace argpppp
{

// TODO: consider using this for all types of option_handler?
//       * At the very least it needs to be possible to create option_handler_result from option_occurrence
// TODO: contain an option (how? by const reference?)
// TODO: contain arguments (how? const char*? string_view?) Can a string_view go back to const char*? If not, don't use it?
export class option_occurrence
{
};

export class callback : public option_handler
{
public:
    explicit callback(std::function<option_handler_result(void)> callback)
        : m_callback([=](option_occurrence) { return callback(); }) {}

    explicit callback(std::function<option_handler_result(option_occurrence)> callback)
        : m_callback(std::move(callback)) {}

    option_handler_result handle_option(const option& /*opt*/, const char* /*arg*/) const override
    {
        // TODO: supply opt
        // TODO: supply arg
        return m_callback(option_occurrence());
    }

private:
    std::function<option_handler_result(option_occurrence)> m_callback;
};

}
