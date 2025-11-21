// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <algorithm>
#include <argp.h>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <vector>

module argpppp;

namespace argpppp
{

options& options::add(const option& o, std::unique_ptr<option_handler> h)
{
    // TODO: split this up into validation and actually doing something

    if (o.sname().is_null())
    {
        if (h)
        {
            // TODO: not sure where this requirement comes from: from reading the documentation it seems legal to have an option with a long name and key=0
            //       Need to investigate, otoh it's not hurting much: it just means you can't have options with key=0
            throw std::invalid_argument("a special option with key = 0 must not have a handler");
        }
    }
    else
    {
        if (!h)
        {
            throw std::invalid_argument("option with key != 0 must have a handler");
        }

        if (!o.sname().is_empty())
        {
            const auto same_short_name = [&o](const auto& owh) { return owh.opt().sname() == o.sname(); };
            if (std::ranges::find_if(m_options, same_short_name) != m_options.end())
            {
                throw std::invalid_argument("option with duplicate short name");
            }
        }
    }

    m_options.emplace_back(o, make_key(o), std::move(h));
    return *this;
}

const option_with_handler* options::find_option(int key) const
{
    auto owh = std::ranges::find_if(m_options, [key](const option_with_handler& o) { return o.key() == key; });
    if (owh == m_options.end())
    {
        return nullptr;
    }

    return &*owh;
}

int options::make_key(const option& o)
{
    if (o.sname().is_empty())
    {
        // TODO: throw if the returned key is >= ARGP_KEY_END? Because we have then too many options?
        // TODO: we should ensure that we do not assign keys that have special meaning for ARGP (ARGP_KEY_xxx constants, except ARGP_KEY_ARG, which is zero, and which is legal)
        return m_next_generated_key++;
    }

    return o.sname().to_char();
}

std::vector<argp_option> get_argp_options(const options& o)
{
    std::vector<argp_option> argp_options;
    argp_options.reserve(o.opts().size() + 1);
    std::transform(o.opts().begin(), o.opts().end(), back_inserter(argp_options), [](const option_with_handler& owh) { return to_argp_option(owh); });
    argp_options.push_back({});
    return argp_options;
}

}
