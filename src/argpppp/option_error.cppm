// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <string>

export module argpppp:option_error;

namespace argpppp
{

// TODO: this type and entire file can go
export class option_error final
{
public:
    explicit option_error(const std::string& message) : m_message(message) {}

    const std::string& message() const { return m_message; }

    bool operator==(const option_error&) const = default;

private:
    std::string m_message;
};

}
