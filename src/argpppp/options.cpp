// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <algorithm>
#include <argp.h>
#include <iterator>
#include <map>
#include <memory>
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

    m_options.emplace_back(o, h);
    return *this;
}

std::vector<argp_option> get_argp_options(const options& o)
{
    std::vector<argp_option> argp_options;
    argp_options.reserve(o.opts().size() + 1);
    std::transform(o.opts().begin(), o.opts().end(), back_inserter(argp_options), [](const option_with_handler& owh) { return to_argp_option(owh.option()); });
    argp_options.push_back({});
    return argp_options;
}

std::map<int, std::shared_ptr<option_handler>> get_option_handlers(const options& o)
{
    std::map<int, std::shared_ptr<option_handler>> handlers;

    for (const auto& option : o.opts())
    {
        if (option.handler())
        {
            handlers[option.option().key()] = option.handler();
        }
    }

    return handlers;
}

}
