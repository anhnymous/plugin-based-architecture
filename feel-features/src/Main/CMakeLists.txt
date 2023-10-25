cmake_minimum_required(VERSION 3.21)

add_library(Main Application.cc VersionHandler.cc HelpHandler.cc MainHandler.cc)
target_link_libraries(Main PUBLIC Interfaces)
target_include_directories(Main PUBLIC ${PROJECT_BINARY_DIR})
target_include_directories(Main INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
