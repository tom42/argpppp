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
    explicit callback(std::function<option_handler_result(void)> callback)
        : m_callback([=](option_occurrence) { return callback(); }) {}

    explicit callback(std::function<option_handler_result(option_occurrence)> callback)
        : m_callback(std::move(callback)) {}

    option_handler_result handle_option(const option& opt, const char* arg) const override
    {
        return m_callback(option_occurrence(opt, arg));
    }

    virtual option_handler_result handle_option(option_occurrence opt) const override
    {
        return m_callback(opt);
    }

private:
    std::function<option_handler_result(option_occurrence)> m_callback;
};

}
