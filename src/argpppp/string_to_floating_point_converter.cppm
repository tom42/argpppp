// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

// Interface to call strtof, strtod, and strtold in a generic way.

module;

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
};

template <>
class string_to_floating_point_converter<double> final
{
public:
    static auto convert(const char* s, char** end)
    {
        return strtod(s, end);
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
};

}
