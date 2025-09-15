// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <stdexcept>

module argpppp;

namespace argpppp
{

options& options::add(const option& o, std::shared_ptr<option_handler> h)
{
    // TODO: tuck away option and its handler, somehow (if we store options without handlers, then we do it in the branch below)

    if (o.key() == 0)
    {
        if (h)
        {
            throw std::invalid_argument("add: special options with key = 0 must not have handlers");
        }
    }
    else
    {
        // TODO: throw if the option should have a handler but has none
    }

    return *this;
}

}
