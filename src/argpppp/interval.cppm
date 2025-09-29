// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

export module argpppp:interval;

namespace argpppp
{

// TODO: does this need a unit test?
//       * default values
//       * non-default values/setters
//       * contains
// TODO: do we call it interval instead? because that's what it is, and to reduce possible confusion with C++ ranges?
template <typename TNumeric>
class interval final
{
public:
    // TODO: default constructor?
    // TODO: setters?

private:
    TNumeric m_min; // TODO: initialize this, no?
    TNumeric m_max; // TODO: initialize this, no?
};

}
