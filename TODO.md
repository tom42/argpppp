<!--
SPDX-FileCopyrightText: 2025 Thomas Mathys
SPDX-License-Identifier: MIT
-->

# TODO
* Improve handling of warning flags:
  * We should not enable warnings always by default, this makes usage for users more difficult.
  * Try something different
    * Have a single interface library whose purpose is to carry buildflags
    * Redo VtgEnableWarnings to add flags as shown in the article below
    * Use VtgEnableWarnings to add build flags to the interface library defined above
    * The targets we want to enable warnings for should now link to this library instead
    * We can now have a build option to enable warnings. It should probably be off by default
      Basically this boils down to setting/not setting warnings on our warnings target
    * We can then go and create presets (for development and CI) where we enable warnings
    * Update the build to make use of this
    * Make a habit of using it locally
  * See
    * https://discourse.cmake.org/t/compiler-warning-flags-for-all-except-externalprojects/4176
    * https://discourse.cmake.org/t/setting-cmake-cxx-flags-within-a-function/4163/7
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
