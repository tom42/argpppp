// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <optional>

export module argpppp:short_name;
import :option_key;

namespace argpppp
{

export class short_name
{
public:
    short_name() = default;

    short_name(char c); // Implicit conversion from char allows for convenient specification of short names.

    static short_name null()
    {
        return short_name(0);
    }

    bool is_empty() const;

    // TODO: test
    bool is_null() const;

    bool is_printable() const;

    // TODO: test
    char to_char() const;

    bool operator==(const short_name&) const = default;

private:
    std::optional<char> m_short_name;
};

}
