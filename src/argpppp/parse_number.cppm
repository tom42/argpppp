// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <concepts>

export module argpppp:parse_number;
import :string_to_integral_converter;

namespace argpppp
{

// TODO: implement, test
ARGPPPP_EXPORT_FOR_UNIT_TESTING
template <std::integral TValue>
bool parse_integral(const char* s, TValue& result, int base)
{
    // TODO: at least one test that base is forwarded
    char* end;
    auto tmp = string_to_integral_converter<TValue>::convert(s, &end, base);

    if constexpr (sizeof(decltype(tmp)) <= sizeof(TValue))
    {
        result = tmp;
    }
    else
    {
        // TODO: we're converting from long (returned by strtol) to something smaller, so we need an additional range check here!
        result = static_cast<TValue>(tmp);
    }

    // TODO: we want more information, right? We can distinguish the following cases:
    //       * GOOD
    //       * TRAILING JUNK
    //       * INVALID NUMERIC STRING
    //       * VALUE TOO SMALL
    //       * VALUE TOO BIG
    //       => Question is, which ones do we actually want/need?
    return true;
}

// TODO: implement, test
template <std::floating_point TValue>
void parse_floating_point(const char* s, TValue& result);

}
