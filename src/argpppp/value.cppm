// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <concepts>
#include <stdexcept>
#include <string>

export module argpppp:value;
import :interval;
import :option;
import :option_handler;
import :option_handler_result;
import :signed_integral_argument_parser;

namespace argpppp
{

export template <typename TValue>
class value
{
public:
    explicit value(TValue)
    {
        // Constructor is required for CTAD of specializations to work, so we cannot work with an undefined primary template.
        // Note: parameter type is intentionally 'TValue' here and not e.g. 'TValue&' as in specializations below.
        // The latter would not be able to match all possible arguments.
        static_assert(false, "only specializations of argpppp::value may be used");
    }
};

template <>
class value<std::string> : public option_handler
{
public:
    explicit value(std::string& target_value) : m_target_value(target_value) {}

    option_handler_result handle_option(const option&, const char* arg) const override
    {
        if (!arg)
        {
            throw std::logic_error("optional arguments are currently not supported");
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

    option_handler_result handle_option(const option&, const char* arg) const override
    {
        if (arg)
        {
            throw std::logic_error("arguments are not supported. value<bool> should be used for switches only");
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

    option_handler_result handle_option(const option& opt, const char* arg) const override
    {
        TValue value;
        auto result = m_parser.parse_arg(opt, arg, value);

        if (result.is_success())
        {
            m_target_variable = value;
        }

        return result;
    }

    value& min(TValue min)
    {
        m_parser.min(min);
        return *this;
    }

    value& max(TValue max)
    {
        m_parser.max(max);
        return *this;
    }

    value& auto_detect_base()
    {
        m_parser.auto_detect_base();
        return *this;
    }

    value& base(int base)
    {
        m_parser.base(base);
        return *this;
    }

private:
    signed_integral_argument_parser<TValue> m_parser;
    TValue& m_target_variable;
};

}
