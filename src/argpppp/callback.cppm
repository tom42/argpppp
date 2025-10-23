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

export class callback : public option_handler
{
public:
    explicit callback(std::function<option_handler_result(const option&, const char*)> callback)
        : m_callback(std::move(callback))
    {
    }

    option_handler_result handle_option(const option& opt, const char* arg) const override
    {
        return m_callback(opt, arg);
    }

private:
    std::function<option_handler_result(const option&, const char*)> m_callback;
};

}
