// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module;

#include <functional>

export module argpppp:option_handlers;

namespace argpppp
{

export class option_handler
{
public:
    // TODO: probably later this needs arguments and a return value
    virtual void handle_option() = 0;

    virtual ~option_handler() {}
};

export class callback : public option_handler
{
public:
    // TODO: get callback functional here and tuck it away (what's its signature?)
    callback(const std::function<void()>& callback) : m_callback(callback) {}

    void handle_option() override
    {
        // TODO: forward to callback
    }

private:
    std::function<void()> m_callback;
};

}
