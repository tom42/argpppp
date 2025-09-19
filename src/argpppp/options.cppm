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
    option_with_handler(const option& o, std::shared_ptr<option_handler> h)
        : m_option(o)
        , m_handler(h)
    {}

    const option& opt() const
    {
        return m_option;
    }

    std::shared_ptr<option_handler> handler() const
    {
        return m_handler;
    }

private:
    argpppp::option m_option;
    std::shared_ptr<option_handler> m_handler;
};

// TODO: with the current API it is possible to set min_args > max_args. Do we care?
export class options final
{
public:
    options& add(const option& o, std::shared_ptr<option_handler> h);

    template <typename TOptionHandler>
    options& add(const option& o, const TOptionHandler& h) requires std::derived_from<TOptionHandler, option_handler>
    {
        add(o, std::make_shared<TOptionHandler>(h));
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

    const std::vector<option_with_handler>& opts() const
    {
        return m_options;
    }

private:
    optional_string m_doc;
    optional_string m_args_doc;
    std::size_t m_min_args = std::numeric_limits<size_t>::min();
    std::size_t m_max_args = std::numeric_limits<size_t>::max();
    std::vector<option_with_handler> m_options;
};

// TODO: document what this does (in particular, options must remain in scope)
ARGPPPP_EXPORT_FOR_UNIT_TESTING
std::vector<argp_option> get_argp_options(const options& o);

// TODO: what happens if an option key is used by more than one option?
// TODO: does this need / have tests?
std::map<int, std::shared_ptr<option_handler>> get_option_handlers(const options& o);

// TODO: does this need a test?
const option& find_option_or_throw(const options& options, int key);

}
