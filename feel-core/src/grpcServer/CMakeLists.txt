cmake_minimum_required(VERSION 3.21)

add_library(grpcServer grpcServer.cc)
target_link_libraries(grpcServer PRIVATE Services)
target_include_directories(grpcServer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
