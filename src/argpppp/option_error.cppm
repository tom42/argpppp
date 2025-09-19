// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <string>

export module argpppp:option_error;

namespace argpppp
{

export class option_error final
{
public:
    explicit option_error(const std::string& message) : m_message(message) {}

    const std::string& message() const { return m_message; }
private:
    std::string m_message;
};

}
