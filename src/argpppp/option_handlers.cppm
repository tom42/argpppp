// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <concepts>
#include <cstdlib>
#include <format>
#include <functional>
#include <limits>
#include <stdexcept>
#include <string>

export module argpppp:option_handlers;
import :interval;
import :option;
import :option_handler_result;
import :optional_string;
import :parse_number;

namespace argpppp
{

export class option_handler
{
public:
    option_handler() = default;
    option_handler(const option_handler&) = default;
    virtual ~option_handler() {}

    virtual option_handler_result handle_option(const char* arg, const option& o) = 0;
};

export class callback : public option_handler
{
public:
    explicit callback(const std::function<option_handler_result(const char*)>& callback) : m_callback(callback) {}

    option_handler_result handle_option(const char* arg, const option&) override
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

    option_handler_result handle_option(const char* arg, const option&) override
    {
        // TODO: do we have to take into account that arg is optional and may be omitted? What do we do then in this case?
        //       note: this is a problem for any option_handler!
        // TODO: note: a conservative solution would be not to support this for the time being!
        m_target_value = arg;
        return ok();
    }

private:
    std::string& m_target_value;
};

template <>
class value<bool> : public option_handler
{
public:
    explicit value(bool& target_variable) : m_target_variable(target_variable) {}

    option_handler_result handle_option(const char*, const option&) override
    {
        m_target_variable = true;
        return ok();
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

    option_handler_result handle_option(const char* arg, const option& option) override
    {
        TValue value;
        auto parse_result = parse_integral(arg, value, m_base);
        switch (parse_result)
        {
            case parse_number_result::success:
                // Success, nothing to do
                break;
            case parse_number_result::underflow:
            case parse_number_result::overflow:
                return out_of_range_error(arg, option);
            case parse_number_result::leading_garbage:
            case parse_number_result::trailing_garbage:
                return error(option, arg, "not a valid integer number");
                break;
            // default:
            //     // TODO: throw exception here or what? (If we wanted to do that we'd have to disable -Wcovered-switch-default)
            //     break;
        }

        if (!m_interval.includes(value))
        {
            return out_of_range_error(arg, option);
        }

        m_target_variable = static_cast<TValue>(value);
        return ok();
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
        if (!is_valid_base(base))
        {
            throw std::invalid_argument("base: invalid base");
        }

        m_base = base;
        return *this;
    }

private:
    option_handler_result out_of_range_error(const char* arg, const option& option) const
    {
        return error(option, arg, std::format("value must be in range [{}, {}]", m_interval.min(), m_interval.max()));
    }

    interval<TValue> m_interval;
    int m_base = 10;
    TValue& m_target_variable;
};

}
