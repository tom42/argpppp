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

// TODO: does this have/need a test
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
        if (!arg)
        {
            throw std::logic_error("value<std::string>: optional arguments are currently not supported");
        }

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

    option_handler_result handle_option(const char* arg, const option&) override
    {
        if (arg)
        {
            throw std::logic_error("value<bool>: arguments are not supported. value<bool> should be used for switches only");
        }

        m_target_variable = true;
        return ok();
    }

private:
    bool& m_target_variable;
};

// Note: std::unsigned_integral is currently not supported due to how strtoul and strtoull handle minus signs.
// A minus sign is not considered bad input but is accepted and the resulting number is cast to unsigned.
// This sounds very broken, so for the time being we don't support it at all.
//
// Also note: if we want to support floating point values we need a separate specialization for that,
// since floating point conversion functions (strtof/strtod/strtold) do not allow to specify a base.
template <std::signed_integral TValue>
class value<TValue> : public option_handler
{
public:
    explicit value(TValue& target_variable) : m_target_variable(target_variable) {}

    option_handler_result handle_option(const char* arg, const option& option) override
    {
        if (!arg)
        {
            throw std::logic_error("value<std::signed_integral>: optional arguments are currently not supported");
        }

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
            default:
                throw std::logic_error("value<std::signed_integral>: unknown parse_number_result");
                break;
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
