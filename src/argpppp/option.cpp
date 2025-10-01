// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <algorithm>
#include <argp.h>
#include <climits>
#include <format>
#include <iterator>
#include <stdexcept>
#include <vector>

module argpppp;
import :interval;

namespace argpppp
{

option::option(int key, const optional_string& name, const optional_string& doc, const optional_string& arg, of flags, int group)
    : m_key(key)
    , m_name(name)
    , m_doc(doc)
    , m_arg(arg)
    , m_flags(flags)
    , m_group(group)
{
    if (need_long_name(key) && !m_name)
    {
        throw std::invalid_argument("option without printable short name needs a long name");
    }
}

bool is_printable_key(int key)
{
    // key may be outside the range [0, 255].
    // Except for EOF, the behavior of isprint() is not defined for values outside this range.
    // We must therefore check the range oursevles first before we can delegate to isprint().
    interval<int> interval(0, UCHAR_MAX);
    return interval.includes(key) && isprint(key);
}

bool need_long_name(int key)
{
    if (key == 0)
    {
        // Special options with key=0 such as documentation options or group headers do not need a long name.
        return false;
    }

    return !is_printable_key(key);
}

std::string get_names(const option& o)
{
    if (is_printable_key(o.key()) && o.name())
    {
        return std::format("'-{:c}' / '--{}'", o.key(), *o.name());
    }

    if (is_printable_key(o.key()))
    {
        return std::format("'-{:c}'", o.key());
    }

    if (o.name())
    {
        return std::format("'--{}'", *o.name());
    }

    throw std::invalid_argument("get_names: option has no name");
}

std::string get_default_error_message(const option& o, const char* arg)
{
    if (!arg)
    {
        arg = "";
    }

    if (o.arg())
    {
        // This results in a somewhat silly message for an option with optional argument when the argument is not given.
        // However, an option with optional argument that fails when the argument is not given is somewhat silly, too.
        return std::format("invalid argument '{}' for option {}", arg, get_names(o));
    }
    else
    {
        return std::format("unexpected option {}", get_names(o));
    }
}

// TODO: unit test
// TODO: make use of this
// TODO: drop this into parser
// TODO: name of additional_info?
// TODO: revisit how this should look: possibly we want to print the default error message followed by the custom one?
//       So we get:
//       * For a switch:                unexpected option '-x'
//       * For an option with argument: invalid argument 'quux' for option '-y'
//       * Switch, with custom message: unexpected option '-x': this is the custom message providing additional information
//       * For an option with argument: invalid argument 'quux' for option '-y': additional info provided by custom message
//       * The special case for option with optional argument which returned an error when the argument is missing:
//         * invalid argument '' for option '-z'
//           - or -
//           invalid argument '' for option '-z': additional info from custom message
//         * As mentioned elsewhere, this case is silly, but then, accepting an optional argument and then returning an error is silly
//       * All of these cases should be tested on parser level (uh, that's silly - can't we test at option level?)
//         * The level is irrelevant insofar was we want to have all of htese cases tested, regardless of where we do so
std::string get_error_message(const option& /*o*/, const char* /*arg*/, const char* /*additional_info*/)
{
    // TODO: implement: probably it's going to look mostly like get_default_error_message above but will append additional_info if given?
    // TODO: replace the term "if (o.arg())" with something like "if (is_switch(o))" (note the inverted logic!)
    throw std::runtime_error("TODO: implement");
}

argp_option to_argp_option(const option& o)
{
    return {c_str(o.name()), o.key(), c_str(o.arg()), to_int(o.flags()), c_str(o.doc()), o.group()};
}

}
