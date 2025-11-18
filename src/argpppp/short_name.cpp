// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module argpppp;

namespace argpppp
{

short_name::short_name(char c) : m_short_name(c) {}

bool short_name::is_empty() const
{
    return !m_short_name.has_value();
}

}
