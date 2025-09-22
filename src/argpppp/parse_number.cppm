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
    // TODO: at least one test that base is forwarded
    parse_integral_result parse_result = parse_integral_result::success;

    // Call strtol or stroull, depending on TValue
    char* end;
    errno = 0;
    auto tmp = string_to_integral_converter<TValue>::convert(s, &end, base);

    // TODO: check end ptr (where does it point to, what does it point to)
    // TODO: theortically this may be overriden by further checks below. Do we care? => Well we can move this test AFTER, may possibly make more sense, no?
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

    if (errno == ERANGE)
    {
        // TODO: also: do we need to fix up a value? (tmp, what we write into the final thing)
        if (tmp == string_to_integral_converter<TValue>::min())
        {
            parse_result = parse_integral_result::underflow;
        }
        else if (tmp == string_to_integral_converter<TValue>::max())
        {
            parse_result = parse_integral_result::overflow;
        }
    }

    if constexpr (sizeof(decltype(tmp)) <= sizeof(TValue))
    {
        value = tmp;
    }
    else
    {
        // We're converting from (unsigned) long as returned by strtol/strtoul to a smaller type.
        // Check for underflow/overflow and clamp the value if necessary.
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

    // TODO: we want more information, right? We can distinguish the following cases:
    //       * TRAILING JUNK (also stuff like "5 x") => So we want to scan to the end of the string, no?
    //       * INVALID NUMERIC STRING
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
