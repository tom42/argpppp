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

    // TODO: decision: throw if c is not either 0 or a printable character (isprint)
    // TODO: question: should we allow printable chars only here? If we do so, is then '\0' also disallowed?
    // TODO: we're currently constructing 'zero' options using short_name(0). Should we have a short_name::zero() factory function again?
    short_name(char c); // Implicit conversion from char allows for convenient specification of short names.

    bool is_empty() const;

    // TODO: implement, test.
    //       When does this return true?
    //       * m_short_name is empty => true
    //       * m_short_name is not empty
    //         * it's 0 => false
    //         * it's printable => false
    //         * It's not printable => true
    // TODO: do we REALLY want to check this?
    // TODO: I think our code using isprint was wrong: the argp_option mentions 'printable ASCII characters', so that would isprint && isascii, no?
    //       => Well maybe, except there IS no isascii function in the standard library
    // TODO: also regarding isprint and consorts: cppreference: ^"To use these functions safely with plain chars (or signed chars), the argument should first be converted to unsigned char:"
    bool requires_long_name() const;

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
