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
import :option_handler;
import :option_with_handler;
import :optional_string;

namespace argpppp
{

// TODO: for the sake of completeness, also add an add_doc helper function?
//       => Well why would we do that - better to remove add_header and instead have
//       => add(header())? => Well the API for add_header is simpler: it is clear you should not add a handler
//       => Same would be true for add_doc()
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
        add(option(short_name::null(), {}, header, {}, {}, group), nullptr);
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

    const option_with_handler* find_option(int argp_key) const;

private:
    int make_argp_key(const option& o);

    optional_string m_doc;
    optional_string m_args_doc;
    std::size_t m_min_args = std::numeric_limits<size_t>::min();
    std::size_t m_max_args = std::numeric_limits<size_t>::max();
    std::vector<option_with_handler> m_options;
    int m_next_generated_key = 256;
};

// Returns a vector of struct argp_option that can be passed to argp_parse.
// The options instance must remain in scope as long as the argp_options are in use.
ARGPPPP_EXPORT_FOR_UNIT_TESTING
std::vector<argp_option> get_argp_options(const options& o);

}
