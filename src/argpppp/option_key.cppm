// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <limits>

export module argpppp:option_key;

namespace argpppp
{

// TODO: unit test
//       * default ctor (and make it constexpr?)
//       * ctor taking char (and make it constexpr?)
export class option_key final
{
public:
    // TODO: currently this creates an option with key=0, that is, a special option
    //       We might want to change this, so that it creates an option with auto-assigned key, since that would probably be the more common use case
    option_key() : m_key(no_short_key) {}

    option_key(char key) : m_key(key) {} // Implicit conversion from char allows for convenient specification of short names.

    // TODO: unit test? And make it constexpr?
    static option_key zero()
    {
        return option_key(0);
    }

    // TODO: purge most uses of to_int: we should only use this once we want to convert to a struct argp_option or however it is called
    int to_int() const
    {
        return m_key;
    }

    bool operator==(const option_key&) const = default;

private:
    static constexpr int no_short_key = std::numeric_limits<int>::min();
    int m_key = 0;
};

// TODO: need to extend the test for this, so that options without a short key are tested too
ARGPPPP_EXPORT_FOR_UNIT_TESTING
bool is_printable_key(int key);

// TODO: need to extend the test for this, so that options without a short key are tested too
ARGPPPP_EXPORT_FOR_UNIT_TESTING
bool need_long_name(option_key key);

}
