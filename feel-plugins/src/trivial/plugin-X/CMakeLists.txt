cmake_minimum_required(VERSION 3.21)

add_library(PluginX SHARED PluginXImpl.cc)
set_target_properties(PluginX
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 1
  POSITION_INDEPENDENT_CODE ON
)
install(TARGETS PluginX LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/provider/plugins)
