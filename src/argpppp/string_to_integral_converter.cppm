// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

// Interface to call strtol, strtoll, strtoul strtoull in a uniform way

module;

#include <climits>
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
        if constexpr (use_strtoll())
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
        if constexpr (use_strtoll())
        {
            return LLONG_MIN;
        }
        else
        {
            return LONG_MIN;
        }
    }

    static constexpr auto max()
    {
        if constexpr (use_strtoll())
        {
            return LLONG_MAX;
        }
        else
        {
            return LONG_MAX;
        }
    }

private:
    static constexpr bool use_strtoll()
    {
        return (sizeof(TResult) >= sizeof(long long)) && !std::is_same_v<TResult, long>;
    }
};

template <std::unsigned_integral TResult>
class string_to_integral_converter<TResult> final
{
public:
    static auto convert(const char* s, char** end, int base)
    {
        if constexpr (use_strtoull())
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
        if constexpr (use_strtoull())
        {
            return 0ull;
        }
        else
        {
            return 0ul;
        }
    }

    static constexpr auto max()
    {
        if constexpr (use_strtoull())
        {
            return ULLONG_MAX;
        }
        else
        {
            return ULONG_MAX;
        }
    }

private:
    static constexpr bool use_strtoull()
    {
        return (sizeof(TResult) >= sizeof(unsigned long long)) && !std::is_same_v<TResult, unsigned long>;
    }
};

}
