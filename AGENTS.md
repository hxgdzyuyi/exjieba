# Development and Contribution Guidelines

This repository hosts the `ExJieba` Elixir library which interfaces with the
Jieba segmentation engine via C++ NIFs.

## Setup
- The submodule `priv/libcppjieba` provides the C++ sources. If it is empty,
  run `git submodule update --init`.

## Building
- Elixir code is compiled using Mix. The custom Mix compiler invokes `make segment`
  to build the native libraries automatically.
- C++ code is formatted with four-space indentation. Elixir code uses two spaces.
- Format Elixir files by running `mix format` before committing.

Keep these steps in mind when modifying the repository to ensure consistency and
working builds.
