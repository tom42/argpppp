// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <concepts>
#include <cstdlib>
#include <format>
#include <functional>
#include <limits>
#include <string>
#include <variant>

export module argpppp:option_handlers;
import :interval;
import :option_error;
import :parse_number;

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
    explicit value(TValue&)
    {
        // Constructor is required for CTAD of specializations to work, so we cannot work with an undefined primary template.
        static_assert(false, "only specializations of argpppp::value may be used");
    }
};

template <>
class value<std::string> : public option_handler
{
public:
    explicit value(std::string& target_value) : m_target_value(target_value) {}

    option_handler_result handle_option(const char* arg) override
    {
        // TODO: do we have to take into account that arg is optional and may be omitted? What do we do then in this case?
        //       note: this is a problem for any option_handler!
        // TODO: note: a conservative solution would be not to support this for the time being!
        m_target_value = arg;
        return true;
    }

private:
    std::string& m_target_value;
};

template <>
class value<bool> : public option_handler
{
public:
    explicit value(bool& target_variable) : m_target_variable(target_variable) {}

    option_handler_result handle_option(const char*) override
    {
        m_target_variable = true;
        return true;
    }

private:
    bool& m_target_variable;
};

// TODO: can we write this in a way it works for
//       * signed integers
//       * all types of floating point values
//         * Not sure how well this can be unified: integer conversions support a base, floating point conversions don't

// Note: std::unsigned_integral is currently not supported due to how strtoul and strtoull handle minus signs.
// A minus sign is not considered bad input but is accepted and the resulting number is cast to unsigned.
// This sounds very broken, so for the time being we don't support it at all.
template <std::signed_integral TValue>
class value<TValue> : public option_handler
{
public:
    explicit value(TValue& target_variable) : m_target_variable(target_variable) {}

    option_handler_result handle_option(const char* arg) override
    {
        // TODO: real implementation: strop parse_integral in here
        //       * Parse value into long (strol or whatever), fail on error
        //         * We fail if end does not point to the terminating character
        //         * Ugh: I think we also need to check errno (e.g. if number is too bug to fit into a long long). See cpppreference
        //       * Check range, fail on error
        //         * This is important, because we'll have to store to e.g. int, but we always parse into long long
        //       * Test entire class (separate unit test, no need to go through parser class)
        TValue value;
        auto parse_result = parse_integral(arg, value, m_base);
        switch (parse_result)
        {
            case parse_number_result::success:
                // Success, nothing to do
                break;
            case parse_number_result::underflow:
            case parse_number_result::overflow:
                // TODO: deduplicate (see below)
                return option_error(std::format("value should be in range [{}, {}]", m_interval.min(), m_interval.max()));
            case parse_number_result::leading_garbage:
            case parse_number_result::trailing_garbage:
                return option_error("meh"); // TODO: real error message
                break;
            // default:
            //     // TODO: throw exception here or what? (If we wanted to do that we'd have to disable -Wcovered-switch-default)
            //     break;
        }

        if (!m_interval.includes(value))
        {
            return option_error(std::format("value should be in range [{}, {}]", m_interval.min(), m_interval.max()));
        }

        m_target_variable = static_cast<TValue>(value);
        return true;
    }

    value& min(TValue min)
    {
        m_interval.min(min);
        return *this;
    }

    value& max(TValue max)
    {
        m_interval.max(max);
        return *this;
    }

    value& auto_detect_base()
    {
        base(0);
        return *this;
    }

    value& base(int base)
    {
        // TODO: do we range-check base here? Probably should do so, so we can detect errors as early as possible, no?
        m_base = base;
        return *this;
    }

private:
    interval<TValue> m_interval;
    int m_base = 10;
    TValue& m_target_variable;
};

}
