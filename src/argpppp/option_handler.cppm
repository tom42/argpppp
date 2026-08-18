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

    // TODO: phase this out, use option occurrence
    virtual option_handler_result handle_option(const option& /*opt*/, const char* /*arg*/) const
    {
        throw "YIKES: this was a pure virtual method and should never be called";
    }

    // TODO: use
    // TODO: make pure virtual
    virtual option_handler_result handle_option(option_occurrence opt) const
    {
        // TODO: temporary implementation
        return handle_option(opt.opt(), opt.c_arg());
    }
};

}
