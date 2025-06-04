# Development and Contribution Guidelines

This repository hosts the `ExJieba` Elixir library which interfaces with the
Jieba segmentation engine via C++ NIFs.

## Setup
- The submodule `priv/libcppjieba` provides the C++ sources. If it is empty,
  run `git submodule update --init`.

Keep these steps in mind when modifying the repository to ensure consistency and
working builds.
No need to build or run tests since the current AGENT environment does not support Elixir.
