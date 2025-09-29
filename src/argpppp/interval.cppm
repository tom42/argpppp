// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <limits>

export module argpppp:interval;

namespace argpppp
{

// TODO: does this need a unit test?
//       * non-default values/setters
//       * contains
ARGPPPP_EXPORT_FOR_UNIT_TESTING
template <typename TNumeric>
class interval final
{
public:
    TNumeric min() const
    {
        return m_min;
    }

    void min(TNumeric min)
    {
        m_min = min;
    }

    TNumeric max() const
    {
        return m_max;
    }

    void max(TNumeric max)
    {
        m_max = max;
    }

    bool includes(TNumeric x) const
    {
        return (min() <= x) && (x <= max());
    }

private:
    TNumeric m_min = std::numeric_limits<TNumeric>::min();
    TNumeric m_max = std::numeric_limits<TNumeric>::max();
};

}
