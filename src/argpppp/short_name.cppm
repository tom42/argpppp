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
    short_name() {}

    // TODO: we're currently constructing 'zero' options using short_name(0). Should we have a short_name::zero() factory function again?
    short_name(char c); // Implicit conversion from char allows for convenient specification of short names.

    bool is_empty() const;

    // TODO: this is only for transitioning, so should go
    option_key to_key() const
    {
        if (!m_short_name)
        {
            return option_key();
        }

        return option_key(m_short_name.value());
    }

private:
    std::optional<char> m_short_name;
};

}
