// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <optional>

export module argpppp:short_name;

namespace argpppp
{

// TODO: unit test
export class short_name
{
public:
    // TODO: test: this yields an option *without short name*
    short_name() {}

    // TODO: question: should we allow printable chars only here? If we do so, is then '\0' also disallowed?
    // TODO: test: this yields an option with short name.
    short_name(char c) : m_short_name(c) {} // Implicit conversion from char allows for convenient specification of short names.

private:
    std::optional<char> m_short_name;
};

}
