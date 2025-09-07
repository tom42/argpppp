// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

// TODO: experiment for a new API: delete once done

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <vector>

import argpppp;

namespace new_api_test
{

using argpppp::of;
using argpppp::optional_string;

// As before this is going to wrap or at least represent argp_option.
// As an idea we might want to reorder constructor arguments.
// Originally we had them ordered as the fields in argp_option,
// but we relatively often don't use the argument flags, and in the
// case of switches we also omit the argument documentation.
// We also often don't specify the group often, so we might want
// to order constructor arguments like this:
//
//   - key
//   - name
//   - doc
//   - arg
//   - flags
//   - group
class option final
{
public:
    option(int key = 0, const optional_string& name = std::nullopt, const optional_string& doc = std::nullopt, const optional_string& arg = std::nullopt, of flags = of::none, int group = 0)
        : m_key(key)
        , m_name(name)
        , m_doc(doc)
        , m_arg(arg)
        , m_flags(flags)
        , m_group(group)
    {}

private:
    int m_key;
    optional_string m_name;
    optional_string m_doc;
    optional_string m_arg;
    of m_flags;
    int m_group;
};

option header(const std::string& doc, int group = 0)
{
    return option({}, {}, doc, {}, {}, group);
}

class option_handler
{
public:
    virtual ~option_handler() {}
};

template <typename TValue>
class value final : public option_handler
{
public:
    value(TValue& v) : m_v(v) {}

    value& min(const TValue& min)
    {
        m_min = min;
        return *this;
    }

    value& max(const TValue& max)
    {
        m_max = max;
        return *this;
    }
private:
    TValue& m_v;
    // TODO: as ridiculous as it is, this compiles if TValue is std::string. Not sure what to make out of that
    TValue m_min = std::numeric_limits<TValue>::min();
    TValue m_max = std::numeric_limits<TValue>::max();
};

template <typename TParam>
class callback final : public option_handler
{
public:
    callback(const std::function<void(TParam)>) {}
};

class option_with_handler final
{
public:
    option_with_handler(const option& o, std::shared_ptr<option_handler> handler)
        : m_o(o),
        m_handler(handler)
    {}

private:
    option m_o;
    std::shared_ptr<option_handler> m_handler;
};

class options final
{
public:
    options& add(const option& o, std::shared_ptr<option_handler> handler)
    {
        m_options.emplace_back(o, handler);
        return *this;
    }

    template <typename THandler>
    options& add(const option& o, const THandler& handler) requires std::derived_from<THandler, option_handler>
    {
        add(o, std::make_shared<THandler>(handler));
        return *this;
    }

    options& doc(const optional_string& doc)
    {
        m_doc = doc;
        return *this;
    }

    options& args_doc(const optional_string& args_doc)
    {
        m_args_doc = args_doc;
        return *this;
    }

    options& nargs(std::size_t nargs)
    {
        m_min_args = m_max_args = nargs;
        return *this;
    }

    options& nargs(std::size_t min_args, std::size_t max_args)
    {
        m_min_args = min_args;
        m_max_args = max_args;
        return *this;
    }

private:
    optional_string m_doc;
    optional_string m_args_doc;
    std::vector<option_with_handler> m_options;
    std::size_t m_min_args = std::numeric_limits<size_t>::min();
    std::size_t m_max_args = std::numeric_limits<size_t>::max();
};

void store_func(int /*x*/) {}

TEST_CASE("new_api_test")
{
    // TODO: ugh. now we have the next problem: If we now go and add a value parameter, then that's fine
    //       except that the header option needs one too. Ugh. Back to the drawing board again.
    //       of course there is a number of ways around this problem
    //       * Go back to non member functions, so we'd have a very generic member function add, but add_header would be a free function: add_header(opts, "Header text", group)
    //       * Make the value thing part of the option class, so the header() function would simply know it doesn' need to set it
    //       * Make the value thing an optional argument of some sorts
    //       * Have an add_header method on the options class itself - why would that be so horrible? It would probably be the absolute exception
    std::string output_file;
    bool verbose = false;
    int compression_level = 0;

    // Some notes:
    // * header() is a function which creates an option instance that represents a group header.
    //   I suppose I could live with passing a null pointer when adding a header.
    //   On the other hand we can still create an add_header() member function.
    //   If we do so I'd probably rename add() to add_option()
    // * As a shorthand for writing option('x', "--xyz", ...) we can use uniform initialization: {'x', "--xyz", ...}
    // * With value() we can store stuff directly in variables, and optionally have min/max values checked
    auto opts = options()
        .doc("Supercruncher 0.0.1 - Copyright (C) tom42, all rights reserved")
        .args_doc("FILE")
        .nargs(1)
        .add(header("General options"), nullptr)
        .add(option('o', "output-file", "Specify output file name", "FILE"), value(output_file))
        .add({ 'v', "verbose", "Print verbose messages" }, value(verbose))
        .add({ 'c', "compression-level", "Specify compression level" }, value(compression_level).min(0).max(10))
        .add({ 'x', {}, "Some other option" }, callback<int>([](int) {})) // TODO: why is it not able to deduce the template argument?
        .add({ 'y', {}, "Another option" }, callback<int>(store_func));
}

}
