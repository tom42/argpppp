// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

// Interface to call strtof, strtod, and strtold in a uniform way

module;

#include <cmath>
#include <concepts>
#include <cstdlib>

export module argpppp:string_to_floating_point_converter;

namespace argpppp
{

ARGPPPP_EXPORT_FOR_UNIT_TESTING
template <std::floating_point TResult>
class string_to_floating_point_converter;

template <>
class string_to_floating_point_converter<float> final
{
public:
    static auto convert(const char* s, char** end)
    {
        return strtof(s, end);
    }

    static constexpr float huge_val()
    {
        return HUGE_VALF;
    }
};

template <>
class string_to_floating_point_converter<double> final
{
public:
    static auto convert(const char* s, char** end)
    {
        return strtod(s, end);
    }

    static constexpr double huge_val()
    {
        return HUGE_VAL;
    }
};

template <>
class string_to_floating_point_converter<long double> final
{
public:
    static auto convert(const char* s, char** end)
    {
        return strtold(s, end);
    }

    static constexpr long double huge_val()
    {
        return HUGE_VALL;
    }
};

}
