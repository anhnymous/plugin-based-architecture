cmake_minimum_required(VERSION 3.21)

add_library(Interfaces Interfaces.cc)
target_link_libraries(Interfaces PUBLIC Plugins)
target_include_directories(Interfaces INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
