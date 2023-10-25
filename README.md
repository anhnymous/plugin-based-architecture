### Introduction
In embedded systems, e.g. IoT, MIoT, Automotive ... it is quite high demand of
deploying multiple different features on tiny embedded devices.

With certain limititations on HW resources, this is quite a challenge. And this
requires special Software/System architectures that tailored for embedded systems.
One of them is the "Plugin-based" architecture.

Basically, this architecture allows us loading/unloading plugins that provide
needed functionalities for features that we install/uninstall in runtime based on
our demands.

### About this project
This project is tiny and simple project (or PoC) which is to demonstrate on how to build
a minimal system that uses "Plugin-base" architecture. This project is aimed to support
embedded Linux systems only.

This project also employs some techniques and concepts like:
`plugin-based architecture`
`gRPC`
`remote invocations`
`dynamic library loading using dlopen APIs`


Directory structures:

design/        brief introduction about the design of this PoC along with some captures
               of simple system design, components design and their relationships.

feel-core/     provide gRPC server and services that forms communication bridges from
               `feel-features` to `feel-plugins`.

feel-features/ features or applications that make use of remote interfaces implemented
               by `feel-plugins`.

feel-plugins/  provides plugins (Linux shared libs, i.e. .so files)

fips/          tells `feel-core` that it wants to install/uninstall features, this module
               is not supported yet in phase 1. The communication between `fips` and
               `feel-core` can be REST APIs.


### Dependencies
#### Conan package grpc/v1.50.0
See: https://github.com/anhnymous/conan-pkgs/tree/master/conan-grpc 

#### Conan package nlohmann-json/v3.11.2
See: https://github.com/anhnymous/conan-pkgs/tree/master/conan-nlohmann-json 


### How to build
#### Build feel-core
```bash
cd feel-core
./build_conan.sh
``` 

#### Build feel-features
```bash
cd feel-features
./build_conan.sh
``` 

#### Build feel-plugins
```
cd feel-plugins
./compile.sh
```


### How to test
#### Start feel-core
```bash
cd feel-core
./test_feel_core.sh (follow instructions)
./feel_core
```

#### Start feel-features
```bash
cd feel-features
./test_feel_feature.sh (follow instructions)
./feel_feature -f X & (start feature X)
./feel_feature -f Y   (start feature Y)
```
