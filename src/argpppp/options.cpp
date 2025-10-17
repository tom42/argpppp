// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <algorithm>
#include <argp.h>
#include <iterator>
#include <map>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <vector>

module argpppp;

namespace argpppp
{

options& options::add(const option& o, std::shared_ptr<option_handler> h)
{
    if (o.key() == 0)
    {
        if (h)
        {
            throw std::invalid_argument("add: a special option with key = 0 must not have a handler");
        }
    }
    else
    {
        if (!h)
        {
            throw std::invalid_argument("add: option must have a handler");
        }
    }

    // TODO: bark if option has a key but it is a duplicate key
    m_options.emplace_back(o, h);
    return *this;
}

const option_with_handler* options::find_option(int key) const
{
    // TODO: review
    auto owh = std::ranges::find_if(m_options, [key](const option_with_handler& o) { return o.opt().key() == key; });
    if (owh == m_options.end())
    {
        return nullptr;
    }

    return &*owh;
}

std::vector<argp_option> get_argp_options(const options& o)
{
    std::vector<argp_option> argp_options;
    argp_options.reserve(o.opts().size() + 1);
    std::transform(o.opts().begin(), o.opts().end(), back_inserter(argp_options), [](const option_with_handler& owh) { return to_argp_option(owh.opt()); });
    argp_options.push_back({});
    return argp_options;
}

}
