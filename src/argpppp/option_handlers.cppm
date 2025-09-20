// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <concepts>
#include <cstdlib>
#include <functional>
#include <limits>
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
    explicit callback(const std::function<option_handler_result(const char*)>& callback) : m_callback(callback) {}

    option_handler_result handle_option(const char* arg) override
    {
        return m_callback(arg);
    }

private:
    std::function<option_handler_result(const char*)> m_callback;
};

export template <typename TValue> class value
{
public:
    value(TValue&)
    {
        // Constructor is required for CTAD of specializations to work, so we cannot work with an undefined primary template.
        static_assert(false, "only specializations of argpppp::value may be used");
    }
};

template <std::signed_integral TValue>
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
        //       * Test entire class (separate unit test, no need to go through parser class)
        char* end;
        auto value = strtoll(arg, &end, 10); // TODO: a base of 0 gives support for decimal, octal and hexadecimal. question is, do we want this by default?
        m_target_variable = static_cast<TValue>(value);
        return true;
    }

    value& min(TValue min)
    {
        m_min = min;
        return *this;
    }

    value& max(TValue max)
    {
        m_max = max;
        return *this;
    }

private:
    TValue m_min = std::numeric_limits<TValue>::min();
    TValue m_max = std::numeric_limits<TValue>::max();
    TValue& m_target_variable;
};

}
