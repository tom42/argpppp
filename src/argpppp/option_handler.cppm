// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

export module argpppp:option_handler;
import :option;
import :option_handler_result;

namespace argpppp
{

export class option_handler
{
public:
    option_handler() = default;
    option_handler(const option_handler&) = default;

    virtual ~option_handler() = default;

    virtual option_handler_result handle_option(option_occurrence opt) const = 0;
};

}
