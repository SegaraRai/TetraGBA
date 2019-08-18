#!/bin/bash

rm -rf build-release
mkdir build-release
cd build-release

cmake -DRELEASE_BUILD=ON ../src
make -j
