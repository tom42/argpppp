# TODO
* Improve handling of warning flags:
  * Wrap up builds.yml for the time being
    * Can we speed up windows by somehow installing Catch2, e.g. using vcpkg?
  * Try something different
    * Test it:
      * Can we use libraries when compiling a program with clang or g++ that have been produced with either of clang/g++?
* Install target:
  * What to do on targets where argp-standalone is used (e.g. cygwin?)
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
