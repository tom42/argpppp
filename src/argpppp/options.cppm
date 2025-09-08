// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

export module argpppp:options;
import :optional_string;

namespace argpppp
{

export class options final
{
public:
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

private:
    optional_string m_doc;
    optional_string m_args_doc;
};

}