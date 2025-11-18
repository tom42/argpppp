// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <cctype>
#include <stdexcept>

module argpppp;

namespace argpppp
{

namespace
{

bool is_valid_short_name(char c)
{
    // cppreference:
    // "To use [ctype] functions safely with plain chars (or signed chars), the argument should first be converted to unsigned char"
    return (c == '\0') || std::isprint(static_cast<unsigned char>(c));
}

}

short_name::short_name(char c)
    : m_short_name(c)
{
    if (!is_valid_short_name(c))
    {
        throw std::invalid_argument("short name must be a printable character or NUL");
    }
}

bool short_name::is_empty() const
{
    return !m_short_name.has_value();
}

}
