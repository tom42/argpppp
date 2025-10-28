// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <format>
#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>

export module argpppp:set;
import :interval;
import :option_handler;
import :parse_number;

namespace argpppp
{

// TODO: do we need to export this?
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
    set(setter_callable<std::string> auto setter) : m_setter(setter) {}

    option_handler_result handle_option(const option&, const char* arg) const override
    {
        if (!arg)
        {
            throw std::logic_error("set<std::string>: optional arguments are currently not supported");
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
    set(setter_callable<bool> auto setter) : m_setter(setter) {}

    option_handler_result handle_option(const option&, const char* arg) const override
    {
        if (arg)
        {
            throw std::logic_error("set<bool>: arguments are not supported. set<bool> should be used for switches only");
        }

        m_setter(true);
        return ok();
    }

private:
    std::function<void(bool)> m_setter;
};

// TODO: specialization for std::signed_integral (do not forget to use setter_callable)
//       * Note: it would be totally awesome if we could not repeat all of the production code and test code we already have for value<std::signed_integral>
//       * Actually, value<std::signed_integral> does very little: the bulk of the work is done by by parse_integral
//         * Now, do we change parse_integral, so that it directly returns option_handler_result instead of parse_number_result?
//           * Well that's one way to go about it
//           * Another one would be to implement set<std::signed_integral> and value<std::signed_integral> with a common base class and template method pattern
//           * And another one would be to wrap parse_integral into another class which does the conversion from raw number parsing to option_handler_result with range check
// TODO: ideally this shares most production and test code with value<std::signed_integral>
//       * Copypaste code from value<std::signed_integral>
//       * Set up a very basic test (should really just test success/error)
//       * Extract common code from value<std::signed_integral> (that has still a full blown test)
//       * Move relevant test code from value<>'s test to the new code's test
//       * Simplify remaining test for value<>
template <std::signed_integral TValue>
class set<TValue> : public option_handler
{
public:
    set(setter_callable<int> auto setter) : m_setter(setter) {}

    option_handler_result handle_option(const option& opt, const char* arg) const override
    {
        // TODO: throw if arg not given (optional args currently not supported)
        // TODO: parse. On success, call setter. Otherwise, don't
        // TODO: duplicated code. factor out and test separately? (also in value<signed_integral> and its test
        if (!arg)
        {
            throw std::logic_error("set<std::signed_integral>: optional arguments are currently not supported");
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
            return out_of_range_error(opt, arg);
        case parse_number_result::leading_garbage:
        case parse_number_result::trailing_garbage:
            return error(opt, arg, "not a valid integer number");
            break;
        default:
            throw std::logic_error("set<std::signed_integral>: unknown parse_number_result");
            break;
        }

        if (!m_interval.includes(value))
        {
            return out_of_range_error(opt, arg);
        }

        m_setter(value);
        return ok();
    }

    // TODO: add all required setters and getters (see value<signed_integral>

private:
    option_handler_result out_of_range_error(const option& opt, const char* arg) const
    {
        return error(opt, arg, std::format("value must be in range [{}, {}]", m_interval.min(), m_interval.max()));
    }

    interval<TValue> m_interval;
    int m_base = 10;
    std::function<void(TValue)> m_setter;
};

}
