// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

module argpppp;

// TODO: implement to_argp_option here.
namespace argpppp
{

argp_option to_argp_option(const option_with_handler& owh)
{
    return
    {
        c_str(owh.opt().name()),
        owh.argp_key(),
        c_str(owh.opt().arg()),
        to_int(owh.opt().flags()),
        c_str(owh.opt().doc()),
        owh.opt().group()
    };
}

}
