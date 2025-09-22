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
    // TODO: add more result codes here. What if parse_integral returned std::expect? Then again, why bother?
};

// TODO: implement, test
ARGPPPP_EXPORT_FOR_UNIT_TESTING
template <std::integral TValue>
parse_integral_result parse_integral(const char* s, TValue& value, int base)
{
    // TODO: do we want a check for valid base? (cppreference: The set of valid values for base is {0, 2, 3, ..., 36})
    // TODO: at least one test that base is forwarded
    parse_integral_result parse_result = parse_integral_result::success;
    char* end;

    errno = 0;
    auto tmp = string_to_integral_converter<TValue>::convert(s, &end, base);

    // TODO: check end ptr (where does it point to, what does it point to)

    // TODO: check errno
    if (errno == ERANGE)
    {
        if (tmp == LLONG_MIN) // TODO: do we not need to templatize this? Get it off the converter?
        {
            parse_result = parse_integral_result::underflow;
        }
        // TODO: set parse_result accordingly
        // TODO: also: do we need to fix up a value?
    }

    if constexpr (sizeof(decltype(tmp)) <= sizeof(TValue))
    {
        value = tmp;
    }
    else
    {
        // TODO: we're converting from long (returned by strtol) to something smaller, so we need an additional range check here!
        //       To be consistent:
        //       * If in range: do nothing
        //       * If too small: correct output to min of TValue and set "too small" / "out of range"
        //       * If too big: correct output to max of TValue and set "too big" / "out of range"
        value = static_cast<TValue>(tmp);
    }

    // TODO: we want more information, right? We can distinguish the following cases:
    //       * GOOD
    //       * TRAILING JUNK (also stuff like "5 x") => So we want to scan to the end of the string, no?
    //       * INVALID NUMERIC STRING
    //       * VALUE TOO SMALL
    //       * VALUE TOO BIG
    //       => Question is, which ones do we actually want/need?
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
