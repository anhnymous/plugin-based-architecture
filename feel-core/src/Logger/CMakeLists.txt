cmake_minimum_required(VERSION 3.21)

add_library(Logger LoggerImpl.cc LogBuffer.cc)
target_include_directories(Logger PUBLIC ${PROJECT_BINARY_DIR})
target_include_directories(Logger INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
