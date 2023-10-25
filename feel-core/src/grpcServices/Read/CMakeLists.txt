cmake_minimum_required(VERSION 3.21)

project(grpcServiceRead)
set(CMAKE_CXX_STANDARD 17)

find_program(_PROTOBUF_PROTOC protoc)
if (NOT _PROTOBUF_PROTOC)
  message(FATAL_ERROR "protoc not found!")
else()
  message(STATUS "Found protoc at: ${_PROTOBUF_PROTOC}")
endif()

find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
if (NOT _GRPC_CPP_PLUGIN_EXECUTABLE)
  message(FATAL_ERROR "grpc_cpp_plugin not found!")
else()
  message(STATUS "Found agrpc_cpp_plugint: ${_GRPC_CPP_PLUGIN_EXECUTABLE}")
endif()

get_filename_component(read_proto_file "protofiles/Read.proto" ABSOLUTE)
get_filename_component(read_proto_path "${read_proto_file}" PATH)

set(read_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/Read.pb.cc")
set(read_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/Read.pb.h")
set(read_grpc_srcs  "${CMAKE_CURRENT_BINARY_DIR}/Read.grpc.pb.cc")
set(read_grpc_hdrs  "${CMAKE_CURRENT_BINARY_DIR}/Read.grpc.pb.h")

add_custom_command(
  OUTPUT "${read_proto_srcs}" "${read_proto_hdrs}" "${read_grpc_srcs}" "${read_grpc_hdrs}"
  COMMAND ${_PROTOBUF_PROTOC}
  ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
       --cpp_out  "${CMAKE_CURRENT_BINARY_DIR}"
       -I "${read_proto_path}"
       --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
       "${read_proto_file}"
  DEPENDS "${read_proto_file}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_library(grpcServiceRead
  ${read_proto_srcs}
  ${read_proto_hdrs}
  ${read_grpc_srcs}
  ${read_grpc_hdrs})

target_link_libraries(grpcServiceRead PUBLIC
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

target_include_directories(grpcServiceRead PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB WL_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/Read.*.h")
foreach(h IN LISTS WL_HEADERS)
  add_custom_command(
    TARGET grpcServiceRead POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${h} ${CMAKE_BINARY_DIR}/include/
    COMMENT "Copied header: ${h}")
endforeach()
