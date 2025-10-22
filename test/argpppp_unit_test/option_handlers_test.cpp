// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <cstdint>
#include <limits>
#include <string>
#include <array> // TODO: for prototyping, remove
#include <span> // TODO: for prototyping, remove
#include <functional> // TODO: for prototyping, remove
#include <iostream> // TODO: for prototyping, remove

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
// Some interesting post: https://stackoverflow.com/questions/61641848/is-it-possible-to-define-a-callable-concept-that-includes-functions-and-lambdas

// Name? _callable is fine, but storer? Well if we call it store_callable, then store is a verb and it says "store [the] callable", which is not what we want to say.
template <typename TCallable, typename TValue>
concept storer_callable = std::is_invocable_v<TCallable, TValue>;

void int_storer(int)
{
    std::cout << "int_storer was called\n";
}

template <typename TValue>
class store_old : public argpppp::option_handler
{
public:
    // TODO: should we use auto&& here instead like people seem to do? Why do people do this?
    explicit store_old(storer_callable<TValue> auto callable) : m_storer(callable) {}

    option_handler_result handle_option(const argpppp::option& /*opt*/, const char* /*arg*/) const override
    {
        // 666 is a bogus value here: in a specialization we'd know (do we?) it is a specialization for int and have the necessary const char* to int conversion code, no?
        m_storer(666);
        return argpppp::ok();
    }

private:
    std::function<void(TValue)> m_storer;
};

template <typename TValue>
void store2(storer_callable<TValue> auto&& /*callable*/) {}

template<typename F, typename R, typename... Args>
concept callable = std::is_invocable_r_v<R, F, Args...>;

template<typename F>
concept runnable = callable<F, void>;

template<typename F, typename R>
concept supplier = callable<F, R>;

template<typename F, typename... Args>
concept consumer = callable<F, void, Args...>;

template<typename F, typename... Args>
concept predicate = callable<F, bool, Args...>;

// TODO: why are people using auto&& rather than simply auto for their callables?
template<typename T, std::size_t Size>
auto test_all(std::span<T, Size> values, predicate<T> auto&& test) {
    for (const auto& value : values) {
        if (not test(value)) {
            return false;
        }
    }
    return true;
}

template <typename TValue> void store3(predicate<TValue> auto&& /*test*/) {}

bool is_even(int number) {
    return number % 2 == 0;
}

TEST_CASE("store, attempt #1")
{
    store_old<int> x(int_storer); // Sigh: CTAD does simply not work, or I cannot get it to work
    store2<int>(int_storer); // Sigh: CTAD does simply not work, or I cannot get it to work

    // Return type, function type, args
    static_assert(std::is_invocable_r_v<void, void(int), int>);
    static_assert(std::is_invocable_r_v<void, int(int), int>); // We can treat a function returning int as returning void. That is, we can ignore the return value.

    // Function type, args
    // If we do not care about the return type anyway we can use is_invocable instead of is_invocable_r
    static_assert(std::is_invocable_v<void(int), int>);
    static_assert(std::is_invocable_v<int(int), int>);

    const auto values = std::array{ 0, 2, 4 };
    const auto span = std::span{ values };

    // OK: the reason this doeas build is because T is deduced from the first argument, not the second one.
    test_all(span, is_even);
    test_all(span, &is_even);
    test_all(span, [&values](auto value) {
        return value == values[0];
        });

    //store3(is_even); // Does not build either
    store3<bool>(is_even); // That builds too

    // OK, so this is what we can get: we will have to specify the type. I guess I'll have to live with that.
    store_old<int> s1(int_storer); // Works, for function
    store_old<double> s2([](double) { std::cout << "storer taking double was called\n"; }); // Works, for function object
    store_old<float> s3([](auto) { std::cout << "storer taking float was called\n"; }); // Works, for function object, at least we can use auto so we do not have to repeat the type name

    argpppp::option bogus{};
    s1.handle_option(bogus, "bogus");
    s2.handle_option(bogus, "bogus");
    s3.handle_option(bogus, "bogus");

    // TODO: it would be interesting (well, rather important) to see whether we can complete our example. Basically we want the following
    //       * A specialization for store<string>: this simply takes arg, converts it to string and passes it to the storer function
    //       * A specialization for store<bool>: this ignores arg (possibly throws if it is given) and calls the storer function with "true"
    //       * A specialization for store<signed_integral>: this takes arg, converts it to the integer type and calls the storer function with that value
}

// Base template, forbids instantiation
template <typename TValue>
class store
{
public:
    // TODO: auto or auto&&?
    explicit store(storer_callable<TValue> auto callable)
    {
        static_assert(false, "only specializations of argpppp::store may be used");
    }
};

TEST_CASE("store, attempt #2")
{
    //store<int> foo([](double) {}); // Does not compile due to static_assert 'only specializations of argpppp::store may be used'
}

// -----------------------------------------------------------------------------

}
