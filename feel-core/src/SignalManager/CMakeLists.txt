cmake_minimum_required(VERSION 3.21)

add_library(SignalManager SignalManager.cc SignalHandlers.cc)
target_include_directories(SignalManager INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(SignalManager PUBLIC FCoreLogger ExitManager)
