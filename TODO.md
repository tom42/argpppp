<!--
SPDX-FileCopyrightText: 2025 Thomas Mathys
SPDX-License-Identifier: MIT
-->

# TODO
* Improve handling of warning flags:
  * Development preset:
    * Enable testing
    * Enable warnings
    * Should we disable testing by default in the CMakeLists.txt?
  * Well the previous attempt with a special project I do not like. Use something different
  * Try something different
    * We can then go and create presets (for development and CI) where we enable warnings
    * Update the build to make use of this
    * Make a habit of using it locally
    * Update VtgEnableWarnings in vtgcmake
    * Test it:
      * Can we use libraries when compiling a program with clang or g++ that have been produced with either of clang/g++?
* Install target:
  * What to do on targets where argp-standalone is used (e.g. cygin?)
    * Install argp-standalone
      * Does argp-standalone need an install target for this?
    * Provide argp-standalone objects and include those in argpppp
* When we use argpppp in another project:
  * Can we choose whether to use argp-standalone or not?
  * Should we forbid installation if we're not the main project?
* reuse-lint target: can we invoke that from the CI builds?
* Write README.md / documentation / example
  * Minimal example
  * More advanced example(s)
    * How to use all (most of) the APIs we currently provide
    * How to use argp_program_version
