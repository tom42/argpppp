// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

// Interface to call strtol, strtoll, strtoul strtoull in a generic way

module;

#include <concepts>
#include <cstdlib>

export module argpppp:string_to_integral_converter;

namespace argpppp
{

ARGPPPP_EXPORT_FOR_UNIT_TESTING
template <std::integral TResult>
class string_to_integral_converter;

template <std::signed_integral TResult>
class string_to_integral_converter<TResult> final
{
public:
    static auto convert(const char* s, char** end, int base)
    {
        // Select between strtoll and strtol, depending on the size of the desired result type.
        if constexpr (sizeof(TResult) >= sizeof(long long))
        {
            return strtoll(s, end, base);
        }
        else
        {
            return strtol(s, end, base);
        }
    }
};

template <std::unsigned_integral TResult>
class string_to_integral_converter<TResult> final
{
public:
    static auto convert(const char* s, char** end, int base)
    {
        // Select between strtoull and strtoul, depending on the size of the desired result type.
        if constexpr (sizeof(TResult) >= sizeof(unsigned long long))
        {
            return strtoull(s, end, base);
        }
        else
        {
            return strtoul(s, end, base);
        }
    }
};

}
