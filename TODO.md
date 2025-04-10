<!--
SPDX-FileCopyrightText: 2025 Thomas Mathys
SPDX-License-Identifier: MIT
-->

# TODO
* Install target: versioning is done, but needs some testing
* When we use argpppp in another project:
  * Can we easily choose whether to build tests or not? How does e.g. Catch2 do this?
  * Can we choose whether to use argp-standalone or not?
  * Should we forbid installation if we're not the main project?
* reuse compliancy
  * Can we somehow add a custom target to cmake?
  * Can we invoke that from the CI builds?
* Write README.md / documentation / example
  * Minimal example
  * More advanced example(s)
    * How to use all (most of) the APIs we currently provide
    * How to use argp_program_version
