// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <algorithm>
#include <argp.h>
#include <iterator>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <vector>

module argpppp;

namespace argpppp
{

options& options::add(const option& o, std::unique_ptr<option_handler> h)
{
    // TODO: split this up into validation and actually doing something

    if (o.key() == option_key::zero())
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

        // TODO: no to_int here
        // TODO: if an option has no short key (that is, is auto assigned), then we should not throw here, either => Instead we should generate a key.
        //       Problem is, how can we avoid generating keys that the user will later supply by himself?
        //       => Best way for the time being would be to be very restrictive in what keys the user can supply - best to accept only the following:
        //          * zero
        //          * printable characters, because we will not auto-assign these.

        if (o.key().to_int() != INT_MIN) // TODO: obviously that's not how we should do it - we should ask the key whether it's auto assigned => add that method, and unit test it
        {
            // TODO: should find by argp_key here, no?
            // TODO: that, and should also defer that check until we've determined the auto assigned key, no? (Yes, but then we need to filter out options with zero key again)
            //       anyway, do we not want to distinguish between "user supplied a duplicated key" and "we supplied a duplicated key and/or it clashed with an ARGP_KEY_xxx key?)
            if (find_option(o.key().to_int()) != nullptr)
            {
                throw std::invalid_argument("option with duplicate key");
            }
        }
        else
        {
            // TODO: what range do we use to assign keys? 256 and up?
            //       => Yes but for the sake of simplicity, ensure the user cannot assign keys from this range, for the time being
            // TODO: we should ensure that we do not assign keys that have special meaning for ARGP (ARGP_KEY_xxx constants, except ARGP_KEY_ARG, which is zero, and which is legal)
            // TODO: the option has no key assigned - we need to give it one then - problem: the option is const
            //       Number of solutions to this problem:
            //       * Construct a fresh option from scratch
            //       * Make the option class mutable
            //       * Store the real key in option_with_handler, and look that one up
            // TODO: this branch is currently only tested by the entire command line parser test - should probably get its own test, so we can test option assignment
        }
    }

    // TODO: quick hackage: determining the auto-generated key should go into a separate function => and do we test it somehow? => class? function)
    int argp_key = o.key().to_int();
    if (o.key().to_int() == INT_MIN) // TODO: obviously that's not how we should do it - we should ask the key whether it's auto assigned => add that method, and unit test it
    {
        argp_key = m_next_generated_key;
        ++m_next_generated_key;
    }

    // TODO: supply real m_argp_key
    // TODO: no .to_int() here
    m_options.emplace_back(o, argp_key, std::move(h));
    return *this;
}

const option_with_handler* options::find_option(int key) const
{
    auto owh = std::ranges::find_if(m_options, [key](const option_with_handler& o) { return o.argp_key() == key; });
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
    std::transform(o.opts().begin(), o.opts().end(), back_inserter(argp_options), [](const option_with_handler& owh) { return to_argp_option(owh); });
    argp_options.push_back({});
    return argp_options;
}

}
