// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <stdexcept>

module argpppp;

namespace argpppp
{

options& options::add(const option& o, std::shared_ptr<option_handler> h)
{
    // TODO: also tuck away option handler, either here, or in the branch below
    m_options.push_back(o);

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

    return *this;
}

}
