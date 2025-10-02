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
import :optional_string;
import :parse_number;

namespace argpppp
{

// TODO: this is absolutely NOT what we option_handler_result want to be (also remove <variant> above)
// TODO: what we want it to be is this:
//       * an integer return code, 0 meaning success, anything else being an Unixoid error code
//       * an optional error message, which must be fully customizable
//         what we have today is NOT fully customizable, because inside command_line_parser.cpp
//         we call get_error_message, which has some hardbaked rules on how to format error messages. sigh
//export using option_handler_result = std::variant<bool, option_error>;

// TODO: review heavily
// TODO: review: no optional_string in here
// TODO: move into own file
// TODO: unit test
export class option_handler_result
{
public:
    // TODO: unit test
    static option_handler_result success()
    {
        // TODO: have constants for defaults here and use them in all ctors (or make them default args...)
        return option_handler_result(true, {}, {}, {});
    }

    // TODO: unit test
    static option_handler_result error(std::string error_message)
    {
        // TODO: move error message
        // TODO: can we even move T into std::optional_t?
        return option_handler_result(false, EXIT_FAILURE, 0, error_message);
    }

    // TODO: unit test
    bool is_success() const
    {
        return m_is_success != 0;
    }

    // TODO: unit test
    int exit_status() const
    {
        return m_exit_status;
    }

    // TODO: unit test
    int error_number() const
    {
        return m_error_number;
    }

    // TODO: unit test
    const std::string& error_message() const
    {
        return m_error_message;
    }

private:
    // TODO: move error message
    option_handler_result(bool is_success, int exit_status, int error_number, const std::string& error_message)
        : m_is_success(is_success)
        , m_exit_status(exit_status)
        , m_error_number(error_number)
        , m_error_message(error_message)
    {}

    bool m_is_success;
    int m_exit_status;
    int m_error_number;
    std::string m_error_message; // TODO: document why this does not need to be optional?
};

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
        return option_handler_result::error(std::format("value should be in range [{}, {}]", m_interval.min(), m_interval.max()));
    }

    interval<TValue> m_interval;
    int m_base = 10;
    TValue& m_target_variable;
};

}
