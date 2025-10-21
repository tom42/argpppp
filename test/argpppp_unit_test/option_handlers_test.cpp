// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <cstdint>
#include <limits>
#include <string>

import argpppp;

namespace argpppp_unit_test
{

using argpppp::error;
using argpppp::ok;
using argpppp::option_handler_result;

TEST_CASE("callback")
{
    argpppp::option opt('s', {}, {}, "STRING");
    argpppp::callback callback([](const argpppp::option& o, const char* a){ return error(o, a, "horrible error"); });

    auto result = callback.handle_option(opt, "argh!");

    CHECK(result == error("invalid argument 'argh!' for option '-s': horrible error"));
}

TEST_CASE("value<string>")
{
    std::string target;
    argpppp::option opt('s', {}, {}, "STRING");
    argpppp::value value(target);

    SECTION("successful parsing")
    {
        value.handle_option(opt, "arg");

        CHECK(target == "arg");
    }

    SECTION("optional arguments are not supported")
    {
        CHECK_THROWS_MATCHES(
            value.handle_option(opt, nullptr),
            std::logic_error,
            Catch::Matchers::Message("value<std::string>: optional arguments are currently not supported"));
    }
}

TEST_CASE("value<bool>")
{
    bool target = false;
    argpppp::option switch_opt('s');
    argpppp::value value(target);

    SECTION("successful parsing")
    {
        value.handle_option(switch_opt, nullptr);

        CHECK(target == true);
    }

    SECTION("handling arguments is not supported")
    {
        CHECK_THROWS_MATCHES(
            value.handle_option(switch_opt, "arg"),
            std::logic_error,
            Catch::Matchers::Message("value<bool>: arguments are not supported. value<bool> should be used for switches only"));
    }
}

TEST_CASE("value<std::signed_integral>")
{
    constexpr int16_t default_target_value = std::numeric_limits<int16_t>::max();
    constexpr int16_t custom_min = 0;
    constexpr int16_t custom_max = 10;
    int16_t target = default_target_value;
    argpppp::option opt('i', {}, {}, "INTEGER");
    argpppp::value value(target);

    SECTION("successful parsing with default settings")
    {
        CHECK(value.handle_option(opt, "-32768") == ok());
        CHECK(target == -32768);

        CHECK(value.handle_option(opt, "32767") == ok());
        CHECK(target == 32767);
    }

    SECTION("successful parsing with custom minimum and maximum value")
    {
        value.min(custom_min).max(custom_max);

        CHECK(value.handle_option(opt, "0") == ok());
        CHECK(target == 0);

        CHECK(value.handle_option(opt, "10") == ok());
        CHECK(target == 10);
    }

    SECTION("parsed value is out of range, custom minimum and maximum value")
    {
        value.min(custom_min).max(custom_max);

        CHECK(value.handle_option(opt, "-1") == error("invalid argument '-1' for option '-i': value must be in range [0, 10]"));
        CHECK(target == default_target_value);

        CHECK(value.handle_option(opt, "11") == error("invalid argument '11' for option '-i': value must be in range [0, 10]"));
        CHECK(target == default_target_value);
    }

    SECTION("parsed value is out of range, range limited by type")
    {
        CHECK(value.handle_option(opt, "-32769") == error("invalid argument '-32769' for option '-i': value must be in range [-32768, 32767]"));
        CHECK(target == default_target_value);

        CHECK(value.handle_option(opt, "32768") == error("invalid argument '32768' for option '-i': value must be in range [-32768, 32767]"));
        CHECK(target == default_target_value);
    }

    SECTION("successful parsing with auto-detection of base")
    {
        value.auto_detect_base();

        CHECK(value.handle_option(opt, "010") == ok());
        CHECK(target == 8);

        CHECK(value.handle_option(opt, "0x10") == ok());
        CHECK(target == 16);
    }

    SECTION("successful parsing with non-standard base")
    {
        value.base(6);

        CHECK(value.handle_option(opt, "20") == ok());
        CHECK(target == 12);
    }

    SECTION("garbage input")
    {
        CHECK(value.handle_option(opt, "!?*") == error("invalid argument '!?*' for option '-i': not a valid integer number"));
        CHECK(target == default_target_value);
    }

    SECTION("auto-detection of base is off by default")
    {
        CHECK(value.handle_option(opt, "0x10") == error("invalid argument '0x10' for option '-i': not a valid integer number"));
        CHECK(target == default_target_value);
    }

    SECTION("invalid base")
    {
        CHECK_THROWS_MATCHES(
            value.base(1),
            std::invalid_argument,
            Catch::Matchers::Message("base: invalid base"));
    }

    SECTION("optional arguments are not supported")
    {
        CHECK_THROWS_MATCHES(
            value.handle_option(opt, nullptr),
            std::logic_error,
            Catch::Matchers::Message("value<std::signed_integral>: optional arguments are currently not supported"));
    }
}

// --- TODO: prototype code below ---------------------------------------------
void int_storer(int) {}
class store : public argpppp::option_handler
{
public:
private:
    option_handler_result handle_option(const argpppp::option& /*opt*/, const char* /*arg*/) const override
    {
        return argpppp::ok();
    }
};
TEST_CASE("store")
{
    store x;

    // Return type, function type, args
    static_assert(std::is_invocable_r_v<void, void(int), int>);
    static_assert(std::is_invocable_r_v<void, int(int), int>); // We can treat a function returning int as returning void. That is, we can ignore the return value.

    // Function type, args
    // If we do not care about the return type anyway we can use is_invocable instead of is_invocable_r
    static_assert(std::is_invocable_v<void(int), int>);
    static_assert(std::is_invocable_v<int(int), int>);
}
// -----------------------------------------------------------------------------

}
