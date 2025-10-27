// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>

export module argpppp:set;
import :option_handler;

namespace argpppp
{

// TODO: do we need to export this?
template <typename TCallable, typename TValue>
concept setter_callable = std::is_invocable_v<TCallable, TValue>;

export template <typename TValue>
class set
{
public:
    explicit set(TValue)
    {
        // Constructor is required for CTAD of specializations to work, so we cannot work with an undefined primary template.
        static_assert(false, "only specializations of argpppp::set may be used");
    }
};

template <>
class set<std::string> : public option_handler
{
public:
    // TODO: do we take the callable by && or what? (currently: by value)
    set(setter_callable<std::string> auto setter) : m_setter(setter) {}

    option_handler_result handle_option(const option&, const char* arg) const override
    {
        if (!arg)
        {
            throw std::logic_error("set<std::string>: optional arguments are currently not supported");
        }

        m_setter(arg);
        return ok();
    }

private:
    std::function<void(std::string)> m_setter;
};

// TODO: specialization for bool (do not forget to use setter_callable)

// TODO: specialization for std::signed_integral (do not forget to use setter_callable)

}
