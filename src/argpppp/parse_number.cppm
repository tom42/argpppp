// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <cerrno>
#include <concepts>
#include <string>

export module argpppp:parse_number;
import :string_to_integral_converter;

namespace argpppp
{

ARGPPPP_EXPORT_FOR_UNIT_TESTING
enum class parse_integral_result
{
    success,
    underflow,
    overflow,
    invalid_numeric_string,
    trailing_garbage
};

ARGPPPP_EXPORT_FOR_UNIT_TESTING
template <std::integral TValue>
parse_integral_result parse_integral(const char* s, TValue& value, int base)
{
    // TODO: do we want a check for valid base? (cppreference: The set of valid values for base is {0, 2, 3, ..., 36})
    parse_integral_result parse_result = parse_integral_result::success;

    // Call strtol/strtoll/strtoul/strtoull, depending on TValue.
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
template <std::floating_point TValue>
void parse_floating_point(const char* s, TValue& value);

}
