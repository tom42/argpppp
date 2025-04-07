<!--
SPDX-FileCopyrightText: 2025 Thomas Mathys
SPDX-License-Identifier: MIT
-->

# argpppp
argpppp is a simple C++ wrapper around glibc's Argp program argument
parsing library. Its key features are:
* It aims to provide a C++ API that is simpler to use than glibc's
  `argp_parse` function
* It comes as a C++ module
* It can use [argp-standalone](https://github.com/tom42/argp-standalone)
  if glibc is not available
