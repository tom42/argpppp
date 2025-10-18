// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <argp.h>
#include <concepts>
#include <cstddef>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

export module argpppp:options;
import :option;
import :option_handlers;
import :optional_string;

namespace argpppp
{

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

    // TODO: return handler by reference? (Yes but what if it's null?)
    // TODO: do we want to have this method at all? option_with_handler could provide a handle_option() too, no?
    option_handler* handler() const
    {
        return m_handler.get();
    }

private:
    argpppp::option m_option;
    std::unique_ptr<option_handler> m_handler;
};

export class options final
{
public:
    options& add(const option& o, std::unique_ptr<option_handler> h);

    template <typename TOptionHandler>
    options& add(const option& o, const TOptionHandler& h) requires std::derived_from<TOptionHandler, option_handler>
    {
        add(o, std::make_unique<TOptionHandler>(h));
        return *this;
    }

    options& add_header(const std::string& header, int group = 0)
    {
        add(option({}, {}, header, {}, {}, group), nullptr);
        return *this;
    }

    const optional_string& doc() const
    {
        return m_doc;
    }

    options& doc(const optional_string& doc)
    {
        m_doc = doc;
        return *this;
    }

    const optional_string& args_doc() const
    {
        return m_args_doc;
    }

    options& args_doc(const optional_string& args_doc)
    {
        m_args_doc = args_doc;
        return *this;
    }

    std::size_t min_args() const
    {
        return m_min_args;
    }

    options& min_args(std::size_t min_args)
    {
        m_min_args = min_args;
        return *this;
    }

    std::size_t max_args() const
    {
        return m_max_args;
    }

    options& max_args(std::size_t max_args)
    {
        m_max_args = max_args;
        return *this;
    }

    options& num_args(std::size_t num_args)
    {
        min_args(num_args);
        max_args(num_args);
        return *this;
    }

    // TODO: why do we need this? Consider making all remaining functions that use this to move into this class and remove this?
    const std::vector<option_with_handler>& opts() const
    {
        return m_options;
    }

    const option_with_handler* find_option(int key) const;

private:
    optional_string m_doc;
    optional_string m_args_doc;
    std::size_t m_min_args = std::numeric_limits<size_t>::min();
    std::size_t m_max_args = std::numeric_limits<size_t>::max();
    std::vector<option_with_handler> m_options;
};

// Returns a vector of struct argp_option that can be passed to argp_parse.
// The options instance must remain in scope as long as the argp_options are in use.
ARGPPPP_EXPORT_FOR_UNIT_TESTING
std::vector<argp_option> get_argp_options(const options& o);

}
