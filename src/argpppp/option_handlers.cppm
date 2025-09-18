// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <functional>

export module argpppp:option_handlers;

namespace argpppp
{

// TODO: later use either std::variant or, better, std::expected? Well our semantics don't quite fit into std::expected.
export using option_handler_result = bool;

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
    callback(const std::function<option_handler_result()>& callback) : m_callback(callback) {}

    option_handler_result handle_option() override
    {
        return m_callback();
    }

private:
    std::function<option_handler_result()> m_callback;
};

}
