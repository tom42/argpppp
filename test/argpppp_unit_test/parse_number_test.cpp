// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <concepts>
#include <cstdint>
#include <cstdlib>
#include <type_traits>
#include <utility>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::parse_integral;
using std::make_pair;

TEST_CASE("parse_integral")
{
    SECTION("int64_t, valid values")
    {
        auto testdata = GENERATE(
            make_pair("-9223372036854775808", int64_t(0x8000000000000000)),
            make_pair("9223372036854775807", int64_t(0x7fffffffffffffff)));
        int64_t result;

        CHECK(parse_integral<int64_t>(testdata.first, result, 10) == true);
        CHECK(result == testdata.second);
    }

    SECTION("uint64_t, valid values")
    {
        auto testdata = GENERATE(
            make_pair("0", uint64_t(0)),
            make_pair("18446744073709551615", uint64_t(0xffffffffffffffff)));
        uint64_t result;

        CHECK(parse_integral<uint64_t>(testdata.first, result, 10) == true);
        CHECK(result == testdata.second);
    }

    SECTION("int32_t, valid values")
    {
        auto testdata = GENERATE(
            make_pair("-2147483648", -2147483648),
            make_pair("2147483647", 2147483647));
        int32_t result;

        CHECK(parse_integral<int32_t>(testdata.first, result, 10) == true);
        CHECK(result == testdata.second);
    }

    SECTION("uint32_t, valid values")
    {
        auto testdata = GENERATE(
            make_pair("0", 0u),
            make_pair("4294967295", 0xffffffffu));
        uint32_t result;

        CHECK(parse_integral<uint32_t>(testdata.first, result, 10) == true);
        CHECK(result == testdata.second);
    }

    SECTION("int8_t, valid values")
    {
        auto testdata = GENERATE(
            make_pair("-128", -128),
            make_pair("127", 127));
        int8_t result;

        CHECK(parse_integral<int8_t>(testdata.first, result, 10) == true);
        CHECK(result == testdata.second);
    }

    // TODO: what do we need to test?
    //       * long long, long, small type
    //       * unsigned long long, unsigned long, small type
    //       * Happy path (which type?)
    //       * Min/max value
    //       * Underflow/overflow
    //       * Complete garbage (empty string, junk at beginning of string)
    //       * Junk after input: "5x", "5 x"
}

// TODO: redo stuff below
/*
// TODO: for the time being this is all just prototyping
namespace
{

// TODO: probably silly, but this actually does compile for TValue=bool
//       Question is, probably the range check should be handled specially.
//       There should be NO range check. Instead, it should convert zero to false and nonzero to true.
//       That, or it should not support bool at all.
template <std::integral TValue>
bool parse_integral(const char* s, TValue& result, int base)
{
    char* end;
    auto tmp = argpppp::string_to_integral_converter<TValue>::convert(s, &end, base);

    // TODO: this is missing *all* error handling
    // TODO: missing: error/range check of strtoll/strtol: look at result, errno (and end?)
    // TODO: missing: is there junk at end of input: look at end

    if constexpr (sizeof(decltype(tmp)) <= sizeof(TValue))
    {
        result = tmp;
    }
    else
    {
        // TODO: missing: we're converting from long to something smaller, so we need an additional range check. For starters we simply cast,
        //       but we really need a range check
        result = static_cast<TValue>(tmp);
    }

    return true;
}

template <std::floating_point TResult>
bool parse_floating_point(const char* s, TResult& result)
{
    char* end;
    auto tmp = argpppp::string_to_floating_point_converter<TResult>::convert(s, &end);

    // TODO: this is missing *all* error handling
    // TODO: missing: generic error handling for strtof/strtod/strtold

    result = tmp;
    return true;
}

}

TEST_CASE("parse_number_prototyping")
{
    // TODO: what about char and unsigned char? => It's probably OK, but should receive some testing
    long long ll;
    long l;
    int i;
    short s;
    unsigned long long ull;
    unsigned short us;
    bool b;
    long double ld;
    double d;
    float f;

    CHECK(parse_integral("123", ll, 10) == true);
    CHECK(ll == 123);

    CHECK(parse_integral("456", l, 10) == true);
    CHECK(l == 456);

    CHECK(parse_integral("789", i, 10) == true);
    CHECK(i == 789);

    CHECK(parse_integral("1023", s, 10) == true);
    CHECK(s == 1023);

    CHECK(parse_integral("12345678", ull, 10) == true);
    CHECK(ull == 12345678);

    CHECK(parse_integral("65535", us, 10) == true);
    CHECK(us == 65535);

    CHECK(parse_integral("23", b, 10) == true);
    CHECK(b == true);

    CHECK(parse_integral("0", b, 10) == true);
    CHECK(b == false);

    CHECK(parse_integral("1", b, 10) == true);
    CHECK(b == true);

    CHECK(parse_floating_point("0.03125", ld) == true);
    CHECK(ld == 0.03125);

    CHECK(parse_floating_point("0.125", d) == true);
    CHECK(d == 0.125);

    CHECK(parse_floating_point("0.25", f) == true);
    CHECK(f == 0.25f);
}*/

}