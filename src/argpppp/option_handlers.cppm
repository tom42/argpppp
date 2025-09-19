// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <concepts>
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

export template <typename TValue> class value
{
public:
    // TODO: ctor is required for ctad to work. Why?
    // TODO: if we do require the ctor, how can we make sure the class is not used?
    //       like so we get an error because it is abstract, but maybe a static_assert is better?
    value(TValue&) {}
};

export template <std::signed_integral TValue>
class value<TValue> : public option_handler
{
public:
    value(TValue& target_variable) : m_target_variable(target_variable) {}

    option_handler_result handle_option() override
    {
        // TODO: real implementation
        //       * Parse value into long (strol or whatever), fail on error
        //       * Check range, fail on error
        //       * Tuck away value
        m_target_variable = 123;
        return true;
    }

    // TODO: add min value
    // TODO: add max value
private:
    TValue& m_target_variable;
};

}
