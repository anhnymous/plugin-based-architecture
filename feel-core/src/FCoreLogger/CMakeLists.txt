cmake_minimum_required(VERSION 3.21)

add_library(FCoreLogger FCoreLogger.cc)
target_link_libraries(FCoreLogger PUBLIC Logger ConfManager)
target_include_directories(FCoreLogger INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
