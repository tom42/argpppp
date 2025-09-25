// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <cerrno>
#include <cmath>
#include <concepts>
#include <stdexcept>
#include <string>

export module argpppp:parse_number;
import :string_to_floating_point_converter;
import :string_to_integral_converter;

namespace argpppp
{

// TODO: rename this enum if it turns out we're going to use it for both floating point and integral parsing
ARGPPPP_EXPORT_FOR_UNIT_TESTING
enum class parse_integral_result
{
    success,
    underflow,
    overflow,
    invalid_numeric_string,
    trailing_garbage
};

inline bool is_valid_base(int base)
{
    if ((base == 0) || ((2 <= base) && (base <= 36)))
    {
        return true;
    }

    return false;
}

ARGPPPP_EXPORT_FOR_UNIT_TESTING
template <std::integral TValue>
parse_integral_result parse_integral(const char* s, TValue& value, int base)
{
    if (!is_valid_base(base))
    {
        throw std::invalid_argument("parse_integral: invalid base");
    }

    // Call strtol/strtoll/strtoul/strtoull, depending on TValue.
    parse_integral_result parse_result = parse_integral_result::success;
    char* end;
    errno = 0;
    auto tmp = string_to_integral_converter<TValue>::convert(s, &end, base);

    // Report underflow or overflow errors signalled by strtol/strtoll/strtoul/strtoull to caller.
    if (errno == ERANGE)
    {
        if (tmp == string_to_integral_converter<TValue>::min())
        {
            parse_result = parse_integral_result::underflow;
        }
        else if (tmp == string_to_integral_converter<TValue>::max())
        {
            parse_result = parse_integral_result::overflow;
        }
    }

    // Assign return value of strtol/strtoll/strtoul/strtoull to output variable.
    if constexpr (sizeof(decltype(tmp)) <= sizeof(TValue))
    {
        value = tmp;
    }
    else
    {
        // The type of the output variable is more narrow than the return type of strtol/strtoll/strtoul/strtoull.
        // Check for underflow or overflow, clamp the value if necessary and report underflow or overflow to caller.
        if (tmp < std::numeric_limits<TValue>::min())
        {
            value = std::numeric_limits<TValue>::min();
            parse_result = parse_integral_result::underflow;
        }
        else if (tmp > std::numeric_limits<TValue>::max())
        {
            value = std::numeric_limits<TValue>::max();
            parse_result = parse_integral_result::overflow;
        }
        else
        {
            value = static_cast<TValue>(tmp);
        }
    }

    // Finally check whether input was malformed
    if (end == s)
    {
        parse_result = parse_integral_result::invalid_numeric_string;
    }
    else
    {
        while (isspace(*end))
        {
            ++end;
        }

        if (*end != '\0')
        {
            parse_result = parse_integral_result::trailing_garbage;
        }
    }

    return parse_result;
}

ARGPPPP_EXPORT_FOR_UNIT_TESTING
template <std::integral TValue>
parse_integral_result parse_integral(const std::string& s, TValue& value, int base)
{
    return parse_integral(s.c_str(), value, base);
}

// TODO: implement, test
ARGPPPP_EXPORT_FOR_UNIT_TESTING
template <std::floating_point TValue>
parse_integral_result parse_floating_point(const char* s, TValue& value)
{
    // Call strtof/strtod/strtold, depending on TValue.
    parse_integral_result parse_result = parse_integral_result::success;
    char* end;
    errno = 0;
    value = string_to_floating_point_converter<TValue>::convert(s, &end);

    // Report underflow or overflow errors signalled by strtof/strtod/strtold to caller.
    if (errno == ERANGE)
    {
        if (value == -HUGE_VAL) // TODO: that should not occur according to cppreference? Still, it does happen, so let's handle it I guess.
        {
            parse_result = parse_integral_result::underflow;
        }
        else if (value == HUGE_VAL)
        {
            parse_result = parse_integral_result::overflow;
        }
    }

    // TODO: Check for bad input (leading or trailing garbage)
    //       * Can we share this code somehow with integer parsing?

    return parse_result;
}

ARGPPPP_EXPORT_FOR_UNIT_TESTING
template <std::floating_point TValue>
parse_integral_result parse_floating_point(const std::string& s, TValue& value)
{
    return parse_floating_point(s.c_str(), value);
}

}
