// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

// Interface to call strtol, strtoll, strtoul strtoull in a uniform way

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
        if constexpr (sizeof(TResult) >= sizeof(long long))
        {
            return strtoll(s, end, base);
        }
        else
        {
            return strtol(s, end, base);
        }
    }

    static constexpr auto min()
    {
        if constexpr (sizeof(TResult) >= sizeof(long long))
        {
            // TODO: return what? Test?
            return LLONG_MIN;
        }
        else
        {
            // TODO: return what? Test?
            return LONG_MIN;
        }
    }

    static constexpr auto max()
    {
        if constexpr (sizeof(TResult) >= sizeof(long long))
        {
            // TODO: return what? Test?
            return LLONG_MAX;
        }
        else
        {
            // TODO: return what? Test?
            return LONG_MAX;
        }
    }
};

template <std::unsigned_integral TResult>
class string_to_integral_converter<TResult> final
{
public:
    static auto convert(const char* s, char** end, int base)
    {
        if constexpr (sizeof(TResult) >= sizeof(unsigned long long))
        {
            return strtoull(s, end, base);
        }
        else
        {
            return strtoul(s, end, base);
        }
    }

    static constexpr auto min()
    {
        if constexpr (sizeof(TResult) >= sizeof(unsigned long long))
        {
            // TODO: return what? Test?
            return 0ull;
        }
        else
        {
            // TODO: return what? Test?
            return 0ul;
        }
    }

    static constexpr auto max()
    {
        if constexpr (sizeof(TResult) >= sizeof(unsigned long long))
        {
            // TODO: return what? Test?
            return ULLONG_MAX;
        }
        else
        {
            // TODO: return what? Test?
            return ULONG_MAX;
        }
    }
};

}
