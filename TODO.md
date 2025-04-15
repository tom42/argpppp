<!--
SPDX-FileCopyrightText: 2025 Thomas Mathys
SPDX-License-Identifier: MIT
-->

# TODO
* Read this: https://discourse.cmake.org/t/how-to-use-fetchcontent-correctly-for-building-external-dependencies/3686
  * A well-structured project should ideally provide a project-specific CMake variable that can be passed down to turn
    tests on or off, and it should default to true if the dependency is being built on its own and false if it is not the top level project.
    * So that's what we should really strife for
  * For install targets: what if we simply omit these for the time being if we're not the main project?
  * Maybe gather such snippets of wisdom? (in vtgcmake repository)
* Install target:
  * Versioning is done, but needs some testing
  * What to do on targets where argp-standalone is used (e.g. cygin?)
    * Install argp-standalone
      * Does argp-standalone need an install target for this?
    * Provide argp-standalone objects and include those in argpppp
* When we use argpppp in another project:
  * Can we easily choose whether to build tests or not? How does e.g. Catch2 do this?
  * Can we choose whether to use argp-standalone or not?
  * Should we forbid installation if we're not the main project?
* reuse-lint target: can we invoke that from the CI builds?
* Write README.md / documentation / example
  * Minimal example
  * More advanced example(s)
    * How to use all (most of) the APIs we currently provide
    * How to use argp_program_version
