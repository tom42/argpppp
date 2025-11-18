// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <argp.h>
#include <optional>
#include <string>
#include <vector>

export module argpppp:option;
import :of;
import :option_key;
import :optional_string;
import :short_name;

namespace argpppp
{

export class option final
{
public:
    // Note:      Constructor parameters are not in the same order as argp_option's field.
    //            Constructor parameters are ordered such that those parameters more commonly
    //            used come first. It is rather uncommon to specify any option flags or
    //            the option group, so these two parameters come last.
    // Also note: We intentionally use std::nullopt rather than {} for default arguments.
    //            The latter bugs with g++ 14.2 (odd linker errors which clang++ and MSVC do not produce).
    option(
        short_name short_name = argpppp::short_name::null(), // TODO: do we even need a default argument here? Maybe, maybe not. Question is, if we do change the meaning of default construction, should we still use 'no key' here?
        const optional_string& name = std::nullopt,
        const optional_string& doc = std::nullopt,
        const optional_string& arg = std::nullopt,
        of flags = of::none,
        int group = 0);

    const optional_string& name() const { return m_name; }

    // TODO: remove
    option_key key() const { return m_key; }

    const optional_string& arg() const { return m_arg; }

    of flags() const { return m_flags; }

    const optional_string& doc() const { return m_doc; }

    int group() const { return m_group; }

private:
    short_name m_short_name;
    option_key m_key; // TODO: remove
    optional_string m_name;
    optional_string m_doc;
    optional_string m_arg;
    of m_flags;
    int m_group;
};

ARGPPPP_EXPORT_FOR_UNIT_TESTING
std::string get_names(const option& o);

ARGPPPP_EXPORT_FOR_UNIT_TESTING
std::string get_error_message(const option& o, const char* arg);

// TODO: delete this: we need it on owh, now, actually
// //                 * Yes delete this, but keep/move the comment!
// Converts an option to an argp_option which can be passed to argp_parse.
// Note that the option must not go out of scope while the argp_option is in use.
ARGPPPP_EXPORT_FOR_UNIT_TESTING
argp_option to_argp_option(const option& o);

}
