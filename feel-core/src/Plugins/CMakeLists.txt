cmake_minimum_required(VERSION 3.21)

add_library(Plugins PluginImpl.cc PluginManager.cc)
include_directories("../../../feel-plugins/provider/")
target_link_libraries(Plugins PRIVATE dl)
target_include_directories(Plugins INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
