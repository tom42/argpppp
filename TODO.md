<!--
SPDX-FileCopyrightText: 2025 Thomas Mathys
SPDX-License-Identifier: MIT
-->

# TODO
* See whether we can get vcpkg up and running on windows. If nothing else it would safe us from having Catch2 inside VS solutions all the time.
  * Works, now can we get it to work on github?
* Have an eye on 'import std' support. It still requires to enable experimental cmake features, but it seems they're getting there.
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
* Write README.md / documentation / example (our single example is maybe too big)
  * Minimal example
  * More advanced example(s)
    * How to use all (most of) the APIs we currently provide
      * Use of option ctor vs. uniform initialization?
      * Limiting the number of arguments
      * Features of value<T> (store data in variables)
      * Callback (note: arg may be null for switches and options with optional arguments!)
      * How to write custom option handlers (here too: arg may be null)
    * How to use argp_program_version / argp_program_bug_address

# Better API
* Continue development:
  * Then we can try writing a value class, which converts arguments directly and stores them in fields
  * And the same we'd like to have for delegating to a storer function, I guess
