cmake_minimum_required(VERSION 3.21)

add_library(Plugins PluginX.cc PluginY.cc)
target_link_libraries(Plugins PRIVATE Services)
target_include_directories(Plugins INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
