#!/bin/bash
cd main
mkdir -p build
cd build
rm -rf *
cmake .. -DSIMULATOR=ON
make -j 4
../bin/main
