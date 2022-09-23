[![Build and Unit Test](https://github.com/cpp-core/strand/actions/workflows/build.yaml/badge.svg)](https://github.com/cpp-core/strand/actions/workflows/build.yaml)

# Strands

## At a Glance

## Installation

### Using depends

    git clone https://github.com/cpp-core/depends
	mkdir depends/build && cd depends/build
    CC=clang-mp-11 CXX=clang++-mp-11 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt -DALL_TEST=ON ..
	make strand-check # Run Tests
	make strand       # Build and Install
