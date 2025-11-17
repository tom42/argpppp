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
    option_key() : m_key(generated_key) {}

    option_key(char key) : m_key(key) {} // Implicit conversion from char allows for convenient specification of short names.

    // TODO: unit test? And make it constexpr?
    static option_key zero()
    {
        return option_key(0);
    }

    // TODO: need to extend the test for this, so that options without a short key are tested too
    //       That is:
    //       * Test with default constructed options
    //       * Test with an option constructed using zero()
    bool is_printable() const;

    // TODO: need to extend the test for this, so that options without a short key are tested too
    bool requires_long_name() const;

    // TODO: implement, test
    bool is_generated() const;

    // TODO: test
    // TODO: PROBLEM: ONLY NOW I REALIZE: THE REAL PROBLEM IS
    //       * option_key is violating SRP. It is really two things: the short name and the key
    //         * Possibly it should be both
    char short_name() const
    {
        // TODO: throw if it is not printable? (or assert)
        return static_cast<char>(m_key);
    }

    // TODO: purge most uses of to_int/argp_key: we should only use this once we want to convert to a struct argp_option or however it is called
    int argp_key() const
    {
        return m_key;
    }

    bool operator==(const option_key&) const = default;

private:
    static constexpr int generated_key = std::numeric_limits<int>::min();
    int m_key = 0;
};

}
