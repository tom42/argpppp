// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <memory>
#include <stdexcept>
#include <utility>

export module argpppp:option_with_handler;
import :option;
import :option_handlers;

namespace argpppp
{

ARGPPPP_EXPORT_FOR_UNIT_TESTING
class option_with_handler final
{
public:
    option_with_handler(const option& o, std::unique_ptr<option_handler> h)
        : m_option(o)
        , m_handler(std::move(h))
    {}

    const option& opt() const
    {
        return m_option;
    }

    option_handler_result handle_option(const char* arg) const
    {
        if (m_handler == nullptr)
        {
            throw std::logic_error("handle_option: no option handler set");
        }

        return m_handler->handle_option(m_option, arg);
    }

private:
    argpppp::option m_option;
    std::unique_ptr<option_handler> m_handler;
};

}
