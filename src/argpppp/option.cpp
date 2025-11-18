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

option::option(short_name short_name, const optional_string& name, const optional_string& doc, const optional_string& arg, of flags, int group)
    : m_short_name(short_name)
    , m_key(short_name.to_key())
    , m_name(name)
    , m_doc(doc)
    , m_arg(arg)
    , m_flags(flags)
    , m_group(group)
{
    if (m_short_name.is_empty() && !m_name)
    {
        throw std::invalid_argument("option requires a long name");
    }
}

std::string get_names(const option& o)
{
    if (o.key().is_printable() && o.name())
    {
        return std::format("--{} (-{:c})", *o.name(), o.key().short_name());
    }

    if (o.key().is_printable())
    {
        return std::format("-{:c}", o.key().short_name());
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

}
