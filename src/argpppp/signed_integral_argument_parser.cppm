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
import :option_handler_result;
import :parse_number;

namespace argpppp
{

ARGPPPP_EXPORT_FOR_UNIT_TESTING
template <std::signed_integral TValue>
class signed_integral_argument_parser final
{
public:
    option_handler_result parse_arg(option_occurrence opt, TValue& value) const
    {
        if (!opt.c_arg())
        {
            throw std::logic_error("optional arguments are currently not supported");
        }

        TValue tmp_value;
        auto parse_result = parse_integral(opt.c_arg(), tmp_value, m_base);
        switch (parse_result)
        {
            case parse_number_result::success:
                // Success, nothing to do
                break;
            case parse_number_result::underflow:
            case parse_number_result::overflow:
                return out_of_range_error(opt);
            case parse_number_result::leading_garbage:
            case parse_number_result::trailing_garbage:
                return error(opt, "not a valid integer number");
                break;
            default:
                throw std::logic_error("unknown parse_number_result");
                break;
        }

        if (!m_interval.includes(tmp_value))
        {
            return out_of_range_error(opt);
        }

        value = tmp_value;
        return ok();
    }

    void min(TValue min)
    {
        m_interval.min(min);
    }

    void max(TValue max)
    {
        m_interval.max(max);
    }

    void auto_detect_base()
    {
        base(0);
    }

    void base(int base)
    {
        if (!is_valid_base(base))
        {
            throw std::invalid_argument("invalid base");
        }

        m_base = base;
    }

private:
    option_handler_result out_of_range_error(option_occurrence opt) const
    {
        return error(opt, std::format("value must be in range [{}, {}]", m_interval.min(), m_interval.max()));
    }

    interval<TValue> m_interval;
    int m_base = 10;
};

}
