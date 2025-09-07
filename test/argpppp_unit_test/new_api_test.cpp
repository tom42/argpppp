// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

// TODO: experiment for a new API: delete once done

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <limits>
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

class value_base
{
public:
    virtual ~value_base() {}
};

template <typename T>
class value : public value_base
{
public:
};

class options final
{
public:
    options& add(const option& o)
    {
        m_options.push_back(o);
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
    std::vector<option> m_options;
    std::size_t m_min_args = std::numeric_limits<size_t>::min();
    std::size_t m_max_args = std::numeric_limits<size_t>::max();
};

TEST_CASE("new_api_test")
{
    // TODO: ugh. now we have the next problem: If we now go and add a value parameter, then that's fine
    //       except that the header option needs one too. Ugh. Back to the drawing board again.
    //       of course there is a number of ways around this problem
    //       * Go back to non member functions, so we'd have a very generic member function add, but add_header would be a free function: add_header(opts, "Header text", group)
    //       * Make the value thing part of the option class, so the header() function would simply know it doesn' need to set it
    //       * Make the value thing an optional argument of some sorts
    //       * Have an add_header method on the options class itself - why would that be so horrible? It would probably be the absolute exception
    auto opts = options()
        .doc("Supercruncher 0.0.1 - Copyright (C) tom42, all rights reserved")
        .args_doc("FILE")
        .nargs(1)
        .add(header("General options"))                             // Header is a function which creates an option with the right content
        .add(option('o', "output-file", "Specify output file name", "FILE"))
        .add({'v', "verbose", "Print verbose messages"});           // Shorthand to create an option: use uniform initialization
}

}
