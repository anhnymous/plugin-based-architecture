cmake_minimum_required(VERSION 3.21)

project(grpcServiceWrite)
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

get_filename_component(write_proto_file "protofiles/Write.proto" ABSOLUTE)
get_filename_component(write_proto_path "${write_proto_file}" PATH)

set(write_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/Write.pb.cc")
set(write_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/Write.pb.h")
set(write_grpc_srcs  "${CMAKE_CURRENT_BINARY_DIR}/Write.grpc.pb.cc")
set(write_grpc_hdrs  "${CMAKE_CURRENT_BINARY_DIR}/Write.grpc.pb.h")

add_custom_command(
  OUTPUT "${write_proto_srcs}" "${write_proto_hdrs}"
         "${write_grpc_srcs}" "${write_grpc_hdrs}"
  COMMAND ${_PROTOBUF_PROTOC}
  ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
       --cpp_out  "${CMAKE_CURRENT_BINARY_DIR}"
       -I "${write_proto_path}"
       --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
       "${write_proto_file}"
  DEPENDS "${write_proto_file}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_library(grpcServiceWrite
  ${write_proto_srcs}
  ${write_proto_hdrs}
  ${write_grpc_srcs}
  ${write_grpc_hdrs})

target_link_libraries(grpcServiceWrite PUBLIC
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

target_include_directories(grpcServiceWrite PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB WL_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/Write.*.h")
foreach(h IN LISTS WL_HEADERS)
  add_custom_command(
    TARGET grpcServiceWrite POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${h} ${CMAKE_BINARY_DIR}/include/
    COMMENT "Copied header: ${h}")
endforeach()
