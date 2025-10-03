// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <concepts>
#include <cstdlib>
#include <format>
#include <functional>
#include <limits>
#include <string>

export module argpppp:option_handlers;
import :interval;
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

    // TODO: also take the option here so we can incorporate info from it into error messages if we want to
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
        return option_handler_result::success();
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
        return option_handler_result::success();
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
        TValue value;
        auto parse_result = parse_integral(arg, value, m_base);
        switch (parse_result)
        {
            case parse_number_result::success:
                // Success, nothing to do
                break;
            case parse_number_result::underflow:
            case parse_number_result::overflow:
                return out_of_range_error();
            case parse_number_result::leading_garbage:
            case parse_number_result::trailing_garbage:
                return option_handler_result::error("meh"); // TODO: real error message
                break;
            // default:
            //     // TODO: throw exception here or what? (If we wanted to do that we'd have to disable -Wcovered-switch-default)
            //     break;
        }

        if (!m_interval.includes(value))
        {
            return out_of_range_error();
        }

        m_target_variable = static_cast<TValue>(value);
        return option_handler_result::success();
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
    option_handler_result out_of_range_error() const
    {
        // TODO: this will fail tests, since the old implementation included a standard error message with prefix, which is now not the case anymore
        //       * We need now to supply this ourselves here with the help of the option and get_error_message, which we'll make available to callers
        //         * This would be a good opportunity to revisit the output of get_error_message
        //           * In most cases it's probably fine
        //           * But the missing argument case for an option with optional argument is bothering me
        //             * In this case it should maybe not say "invalid argument '' for option -x"
        //             * Instead maybe just "unexpected option -x". It's still somewhat silly, but it's really hard to provide good defaults
        //       * Also something to consider: how do we name options in the error message.
        //         * Currently we say '-i' if there is only a short name
        //         * And we say '--integer' if there is only a long name
        //         * And we say '-i / --integer' if there is both
        //         => Do we like this, or do we rather use always the long one only IF it is available? Or would that be more confusing?
        return option_handler_result::error(std::format("value should be in range [{}, {}]", m_interval.min(), m_interval.max()));
    }

    interval<TValue> m_interval;
    int m_base = 10;
    TValue& m_target_variable;
};

}
