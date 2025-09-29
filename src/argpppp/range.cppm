// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

export module argpppp:range;

namespace argpppp
{

// TODO: does this need a unit test?
//       * default values
//       * non-default values/setters
//       * contains
// TODO: do we call it interval instead? because that's what it is, and to reduce possible confusion with C++ ranges?
template <typename TValue>
class range final
{
public:
    // TODO: default constructor?
    // TODO: setters?

private:
    TValue m_min; // TODO: initialize this, no?
    TValue m_max; // TODO: initialize this, no?
};

}
