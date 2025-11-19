// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <argp.h>
#include <memory>
#include <stdexcept>
#include <utility>

export module argpppp:option_with_handler;
import :option;
import :option_handler;

namespace argpppp
{

ARGPPPP_EXPORT_FOR_UNIT_TESTING
class option_with_handler final
{
public:
    option_with_handler(const option& o, int argp_key, std::unique_ptr<option_handler> h)
        : m_option(o)
        , m_argp_key(argp_key)
        , m_handler(std::move(h))
    {}

    const option& opt() const
    {
        return m_option;
    }

    int argp_key() const
    {
        return m_argp_key;
    }

    option_handler_result handle_option(const char* arg) const
    {
        if (m_handler == nullptr)
        {
            throw std::logic_error("no option handler set");
        }

        return m_handler->handle_option(m_option, arg);
    }

private:
    argpppp::option m_option;
    int m_argp_key;
    std::unique_ptr<option_handler> m_handler;
};

// Converts an option_with_handler to an argp_option which can be passed to argp_parse.
// Note that the option_with_handler must not go out of scope while the argp_option is in use.
ARGPPPP_EXPORT_FOR_UNIT_TESTING
argp_option to_argp_option(const option_with_handler& owh);

}
