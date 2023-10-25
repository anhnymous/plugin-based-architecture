cmake_minimum_required(VERSION 3.21)

find_package(Threads REQUIRED)

set(protobuf_MODULE_COMPATIPLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
set(_REFLECTION gRPC::grpc++_reflection)
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
