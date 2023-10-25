cmake_minimum_required(VERSION 3.21)

add_library(PluginY SHARED PluginYImpl.cc)
set_target_properties(PluginY
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 1
  POSITION_INDEPENDENT_CODE ON
)
install(TARGETS PluginY LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/provider/plugins)
