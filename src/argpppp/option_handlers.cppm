// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

export module argpppp:option_handlers;

namespace argpppp
{

// TODO: add parsing interface for parser (abstract)
// TODO: can we make this class abstract? How?
export class option_handler
{
public:
    virtual ~option_handler() {}
};

export class callback : public option_handler
{
public:
};

}
