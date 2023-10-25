cmake_minimum_required(VERSION 3.21)

add_library(ConfManager ConfManager.cc)
target_include_directories(ConfManager PUBLIC ${PROJECT_BINARY_DIR})
target_include_directories(ConfManager INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
