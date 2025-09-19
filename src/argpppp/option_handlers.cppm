// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <functional>
#include <variant>

export module argpppp:option_handlers;
import :option_error;

namespace argpppp
{

export using option_handler_result = std::variant<bool, option_error>;

export class option_handler
{
public:
    option_handler() = default;
    option_handler(const option_handler&) = default;
    virtual ~option_handler() {}

    // TODO: probably later this needs (an) argument(s)
    virtual option_handler_result handle_option() = 0;
};

export class callback : public option_handler
{
public:
    explicit callback(const std::function<option_handler_result()>& callback) : m_callback(callback) {}

    option_handler_result handle_option() override
    {
        return m_callback();
    }

private:
    std::function<option_handler_result()> m_callback;
};

}
