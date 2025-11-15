// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <cctype>
#include <climits>

module argpppp;

namespace argpppp
{

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

}
