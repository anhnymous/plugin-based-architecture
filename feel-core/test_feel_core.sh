#!/bin/bash

mkdir -p build-conan/bin/etc
mkdir -p build-conan/bin/logs/fcore
cp config/fcore.conf build-conan/bin/etc
cat << EOF
Now you can perform:
$ cd build-conan/bin
$ export CONFDIR=etc
$ export LD_LIBRARY_PATH=~/.conan/data/grpc/v1.50.0/_/_/package/ad3238178499f1666b19ad3dc9279be28103b4ac/lib/ (deprecated)
$ ./feel_core
EOF

