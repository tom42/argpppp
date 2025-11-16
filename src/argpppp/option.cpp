// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <argp.h>
#include <format>
#include <stdexcept>

module argpppp;
import :interval;

namespace argpppp
{

namespace
{

bool is_switch(const option& o)
{
    return !o.arg();
}

bool is_arg_optional(const option& o)
{
    return (o.flags() & of::arg_optional) != of::none;
}

}

option::option(option_key key, const optional_string& name, const optional_string& doc, const optional_string& arg, of flags, int group)
    : m_key(key)
    , m_name(name)
    , m_doc(doc)
    , m_arg(arg)
    , m_flags(flags)
    , m_group(group)
{
    if (need_long_name(key) && !m_name) // TODO: no to_int here
    {
        throw std::invalid_argument("option without printable short name needs a long name");
    }
}

std::string get_names(const option& o)
{
    if (is_printable_key(o.key().to_int()) && o.name()) // TODO: no to_int here
    {
        return std::format("--{} (-{:c})", *o.name(), o.key().to_int()); // TODO: no to_int here
    }

    if (is_printable_key(o.key().to_int())) // TODO: no to_int here
    {
        return std::format("-{:c}", o.key().to_int()); // TODO: no to_int here
    }

    if (o.name())
    {
        return std::format("--{}", *o.name());
    }

    throw std::invalid_argument("option has no name");
}

std::string get_error_message(const option& o, const char* arg)
{
    if (is_switch(o) || (is_arg_optional(o) && (arg == nullptr)))
    {
        return std::format("unexpected option '{}'", get_names(o));
    }
    else
    {
        if (arg == nullptr)
        {
            arg = "(null)";
        }

        return std::format("invalid argument '{}' for option '{}'", arg, get_names(o));
    }
}

// TODO: delete this: we need it on owh, now, actually
argp_option to_argp_option(const option& o)
{
    // TODO: no to_int here() (well no, that's mostly the only place where we do want it?)
    return {c_str(o.name()), o.key().to_int(), c_str(o.arg()), to_int(o.flags()), c_str(o.doc()), o.group()};
}

}
