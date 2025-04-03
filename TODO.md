<!--
SPDX-FileCopyrightText: 2025 Thomas Mathys
SPDX-License-Identifier: MIT
-->

# TODO
* Release a proper 1.0.3 of argp-standalone where the package version in CMakeLists.txt is correct
  * Change CMakeLists.txt to 1.0.3
  * Make a release 1.0.3
  * Have a checklist for releasing
	* Version number needs to be updated in sources/build scripts
	* Things need to be tagged correctly in git
* Set up github action (do we build release and debug?)
* Go for C++ 23 or 26?
* Upgrade to CMake 4.0.0
* Use import std?
* Take existing functionality, add stuff required for agbpacker and go 1.0.0
* When we use argpppp in another project:
  * Can we easily choose whether to build tests or not? How does e.g. Catch2 do this?
  * Can we choose whether to use argp-standalone or not?
* Write README.md / documentation / example
* Have a test for argp_program_version again?
