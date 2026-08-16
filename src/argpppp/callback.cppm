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
export class option_occurrence
{
public:
    option_occurrence(const option& opt, const char* arg)
        : m_opt(opt)
        , m_arg(arg)
    {}

    const option& opt() const { return m_opt; }

    const char* c_arg() const { return m_arg; }

private:
    const option& m_opt;
    const char* m_arg;
};

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

private:
    std::function<option_handler_result(option_occurrence)> m_callback;
};

}
