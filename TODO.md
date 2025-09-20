<!--
SPDX-FileCopyrightText: 2025 Thomas Mathys
SPDX-License-Identifier: MIT
-->

# TODO
* Maybe get clang format up and running. Possible approach
  * Configure it
  * Make it part of the quality build. If it fails, fail the build
* Install target:
  * What to do on targets where argp-standalone is used (e.g. cygwin?)
    * Install argp-standalone
      * Does argp-standalone need an install target for this?
    * Provide argp-standalone objects and include those in argpppp
* When we use argpppp in another project:
  * Can we choose whether to use argp-standalone or not?
  * Should we forbid installation if we're not the main project?
* Write README.md / documentation / example
  * Minimal example
  * More advanced example(s)
    * How to use all (most of) the APIs we currently provide
    * How to use argp_program_version

# Better API
* Delete new_api_test.cpp code below once done
* Continue development:
  * Our handler base class is far from complete: it needs the argument string, so we can parse it
  * Then we can try writing a value class, which converts arguments directly and stores them in fields
  * And the same we'd like to have for delegating to a storer function, I guess

* ----------new_api_test.cpp--------------------------------------------------------

namespace new_api_test
{

class options final
{
public:
    options& add(const option& o, std::shared_ptr<option_handler> handler)
    {
        m_options.emplace_back(o, handler);
        return *this;
    }

    template <typename THandler>
    options& add(const option& o, const THandler& handler) requires std::derived_from<THandler, option_handler>
    {
        add(o, std::make_shared<THandler>(handler));
        return *this;
    }

private:
    std::vector<option_with_handler> m_options;
};

void store_func(int /*x*/) {}

TEST_CASE("new_api_test")
{
    // TODO: ugh. now we have the next problem: If we now go and add a value parameter, then that's fine
    //       except that the header option needs one too. Ugh. Back to the drawing board again.
    //       of course there is a number of ways around this problem
    //       * Go back to non member functions, so we'd have a very generic member function add, but add_header would be a free function: add_header(opts, "Header text", group)
    //       * Make the value thing part of the option class, so the header() function would simply know it doesn' need to set it
    //       * Make the value thing an optional argument of some sorts
    //       * Have an add_header method on the options class itself - why would that be so horrible? It would probably be the absolute exception
    std::string output_file;
    bool verbose = false;
    int compression_level = 0;

    // Some notes:
    // * header() is a function which creates an option instance that represents a group header.
    //   I suppose I could live with passing a null pointer when adding a header.
    //   On the other hand we can still create an add_header() member function.
    //   If we do so I'd probably rename add() to add_option()
    // * As a shorthand for writing option('x', "--xyz", ...) we can use uniform initialization: {'x', "--xyz", ...}
    // * With value() we can store stuff directly in variables, and optionally have min/max values checked
    auto opts = options()
        .doc("Supercruncher 0.0.1 - Copyright (C) tom42, all rights reserved")
        .args_doc("FILE")
        .nargs(1)
        .add(header("General options"), nullptr)
        .add(option('o', "output-file", "Specify output file name", "FILE"), value(output_file))
        .add({ 'v', "verbose", "Print verbose messages" }, value(verbose))
        .add({ 'c', "compression-level", "Specify compression level" }, value(compression_level).min(0).max(10))
        .add({ 'x', {}, "Some other option" }, callback<int>([](int) {})) // TODO: why is it not able to deduce the template argument?
        .add({ 'y', {}, "Another option" }, callback<int>(store_func));
}

}
