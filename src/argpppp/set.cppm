// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>

export module argpppp:set;
import :interval;
import :option;
import :option_handler;
import :option_handler_result;
import :signed_integral_argument_parser;

namespace argpppp
{

template <typename TCallable, typename TValue>
concept setter_callable = std::is_invocable_v<TCallable, TValue>;

export template <typename TValue>
class set
{
public:
    explicit set(TValue)
    {
        // Constructor is required for CTAD of specializations to work, so we cannot work with an undefined primary template.
        static_assert(false, "only specializations of argpppp::set may be used");
    }
};

template <>
class set<std::string> : public option_handler
{
public:
    explicit set(setter_callable<std::string> auto setter) : m_setter(setter) {}

    option_handler_result handle_option(const option&, const char* arg) const override
    {
        if (!arg)
        {
            throw std::logic_error("optional arguments are currently not supported");
        }

        m_setter(arg);
        return ok();
    }

private:
    std::function<void(std::string)> m_setter;
};

template <>
class set<bool> : public option_handler
{
public:
    explicit set(setter_callable<bool> auto setter) : m_setter(setter) {}

    option_handler_result handle_option(const option&, const char* arg) const override
    {
        if (arg)
        {
            throw std::logic_error("arguments are not supported. set<bool> should be used for switches only");
        }

        m_setter(true);
        return ok();
    }

private:
    std::function<void(bool)> m_setter;
};

// TODO: ideally this shares most production and test code with value<std::signed_integral>
//       * Extract common code from value<std::signed_integral> (that has still a full blown test)
//       * Move relevant test code from value<>'s test to the new code's test
//       * Simplify remaining test for value<>
template <std::signed_integral TValue>
class set<TValue> : public option_handler
{
public:
    explicit set(setter_callable<int> auto setter) : m_setter(setter) {}

    option_handler_result handle_option(const option& opt, const char* arg) const override
    {
        TValue value;
        auto result = m_parser.parse_arg(opt, arg, value);

        if (result.is_success())
        {
            m_setter(value);
        }

        return result;
    }

    set& min(TValue min)
    {
        m_parser.min(min);
        return *this;
    }

    set& max(TValue max)
    {
        m_parser.max(max);
        return *this;
    }

    set& auto_detect_base()
    {
        m_parser.auto_detect_base();
        return *this;
    }

    set& base(int base)
    {
        m_parser.base(base);
        return *this;
    }

private:
    signed_integral_argument_parser<TValue> m_parser;
    std::function<void(TValue)> m_setter;
};

}
