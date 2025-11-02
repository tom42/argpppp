// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <concepts>
#include <format>
#include <stdexcept>

export module argpppp:signed_integral_argument_parser;
import :interval;
import :option;
import :option_handler;
import :parse_number;

namespace argpppp
{

// TODO: unit test: most of the heavy lifting from value<signed_integral>'s test goes here
template <std::signed_integral TValue>
class signed_integral_argument_parser final
{
public:
    signed_integral_argument_parser(const interval<TValue>& interval, int base)
        : m_interval(interval)
        , m_base(base) {
    }

    option_handler_result parse_arg(const option& opt, const char* arg, TValue& value, const char* /*calling_class*/)
    {
        if (!arg)
        {
            // TODO: use calling_class
            throw std::logic_error("set<std::signed_integral>: optional arguments are currently not supported");
        }

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
                // TODO: use calling_class
                throw std::logic_error("set<std::signed_integral>: unknown parse_number_result");
                break;
        }

        if (!m_interval.includes(value))
        {
            return out_of_range_error(opt, arg);
        }

        return ok();
    }

private:
    option_handler_result out_of_range_error(const option& opt, const char* arg) const
    {
        return error(opt, arg, std::format("value must be in range [{}, {}]", m_interval.min(), m_interval.max()));
    }

    interval<TValue> m_interval;
    int m_base = 10;
};

}
