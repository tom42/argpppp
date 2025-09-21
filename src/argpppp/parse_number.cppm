// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <concepts>

export module argpppp:parse_number;

namespace argpppp
{

// TODO: implement, test
template <std::integral TValue>
void parse_integral(const char* s, TValue& result, int base);

// TODO: implement, test
template <std::floating_point TValue>
void parse_floating_point(const char* s, TValue& result);

}
