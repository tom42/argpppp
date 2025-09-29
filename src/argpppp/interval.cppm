// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <limits>

export module argpppp:interval;

namespace argpppp
{

// TODO: does this need a unit test?
//       * default values
//       * non-default values/setters
//       * contains
template <typename TNumeric>
class interval final
{
public:
    void min(TNumeric min)
    {
        m_min = min;
    }

    void max(TNumeric max)
    {
        m_max = max;
    }

private:
    TNumeric m_min = std::numeric_limits<TNumeric>::min();
    TNumeric m_max = std::numeric_limits<TNumeric>::max();
};

}
