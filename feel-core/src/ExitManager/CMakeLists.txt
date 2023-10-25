cmake_minimum_required(VERSION 3.21)

add_library(ExitManager ExitManager.cc ExitHandlers.cc)
target_link_libraries(ExitManager PUBLIC FCoreLogger)
target_include_directories(ExitManager INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
