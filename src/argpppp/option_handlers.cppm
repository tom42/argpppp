// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <concepts>
#include <functional>
#include <cstdlib>
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

    virtual option_handler_result handle_option(const char* arg) = 0;
};

export class callback : public option_handler
{
public:
    explicit callback(const std::function<option_handler_result()>& callback) : m_callback(callback) {}

    option_handler_result handle_option(const char* /*arg*/) override
    {
        // TODO: pass arg to callback. Obviously this requires a change to the callback signature
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

    option_handler_result handle_option(const char* arg) override
    {
        // TODO: real implementation
        //       * Parse value into long (strol or whatever), fail on error
        //         * We fail if end does not point to the terminating character
        //         * Ugh: I think we also need to check errno (e.g. if number is too bug to fit into a long long). See cpppreference
        //       * Check range, fail on error
        //         * This is important, because we'll have to store to e.g. int, but we always parse into long long
        char* end;
        auto value = strtoll(arg, &end, 10); // TODO: a base of 0 gives support for decimal, octal and hexadecimal. question is, do we want this by default?
        m_target_variable = static_cast<TValue>(value);
        return true;
    }

    // TODO: add min value
    // TODO: add max value
private:
    TValue& m_target_variable;
};

}
