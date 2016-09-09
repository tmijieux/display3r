#!/bin/bash
PREFIX=${PREFIX:-$HOME/prefix}

cd $(dirname $0)
mkdir -p ../build $PREFIX
cd ../build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=${PREFIX} -DSANITIZE_UNDEFINED=On -DSANITIZE_ADDRESS=On 
