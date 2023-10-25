########################################################################################
# COPYRIGHT: (c) 2023 Anh Tran
# Author: Anh Tran (anhthd2017@gmail.com)
# File: CMakeLists.txt
# License: GPLv3
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.
########################################################################################

cmake_minimum_required(VERSION 3.21)
project(feel_core VERSION 0.1.0 LANGUAGES C CXX DESCRIPTION "Feel Core")
message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
message(STATUS "PROJECT_VERSION: ${PROJECT_VERSION}")
include(common_cmake/common.cmake)

include(CMakePrintHelpers)
cmake_print_variables(ENABLED_LANGUAGES)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE OFF)

configure_file(${PROJECT_SOURCE_DIR}/src/FCoreVersion.hh.in
               ${PROJECT_BINARY_DIR}/FCoreVersion.hh)

set(FCore_DEFAULT_PORT 9997)
set(FCore_DEFAULT_HOST localhost)
set(CONFDIR CONFDIR)
set(DATADIR DATADIR)
set(BASEDIR BASEDIR)

set(PLUGIN_X "PluginX")
set(PLUGIN_Y "PluginY")
set(PLUGIN_X_PATH "${PROJECT_SOURCE_DIR}/../feel-plugins/provider/plugins/libPluginX.so")
set(PLUGIN_Y_PATH "${PROJECT_SOURCE_DIR}/../feel-plugins/provider/plugins/libPluginY.so")

configure_file(${PROJECT_SOURCE_DIR}/src/FCoreConfig.hh.in
               ${PROJECT_BINARY_DIR}/FCoreConfig.hh)

set(default_build_type "Debug")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Chose the build type." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
               "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if(CMAKE_CROSSCOMPILING)
  set(not_crosscompiling OFF)
else()
  set(not_crosscompiling ON)
endif()

set(POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
option(DEBUG "Build FCore optimized for GDB debugging" OFF)
if(DEBUG)
  message(STATUS "USER: feel_core debug build enabled")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb3 -DDEBUG -DVERBOSE")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb3 -DDEBUG -DVERBOSE")
else()
  message(STATUS "USER: feel_core release build enabled")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftree-vectorize")
endif()

set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

message(STATUS "USER: CMAKE_BUILD_RPATH = ${CMAKE_BUILD_RPATH}")
message(STATUS "USER: CMAKE_INSTALL_RPATH = ${CMAKE_INSTALL_RPATH}")
message(STATUS "USER: CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "USER: CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfloat-equal")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wduplicated-cond")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat-security")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdouble-promotion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wlogical-not-parentheses")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE -Wextra -Wall -Werror")

# Even code from Google and OpenSource community contain error-prone
# implementation. So, never trust code from anyone completely, even Google.
# When we build GRPC, lot of warning with these flags.
unset(GRPC)
if(NOT GRPC)
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnull-dereference")
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -Wsign-conversion")
endif()

set(CODE_COVERAGE)
set(CMAKE_INSTALL_LIBDIR)
set(CMAKE_INSTALL_BINDIR)
set(CMAKE_INSTALL_SBINDIR)
set(CMAKE_INSTALL_INCLUDEDIR)
set(CMAKE_INSTALL_LIBEXECDIR)
set(CMAKE_INSTALL_DATAROOTDIR)
set(CMAKE_INSTALL_OLDINCLUDEDIR)
set(CMAKE_EXPORT_NO_PACKAGE_REGISTRY)

option(CONAN "Using Conan build system" ON)
if (CONAN)
  set(CONAN_DISABLE_CHECK_COMPILER OFF)
  if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
    conan_define_targets()
  endif()
  get_filename_component(PROJ_DIR . ABSOLUTE)
endif()

add_definitions(-DTARGET="${TARGET}")
message(STATUS "USER: build target is ${TARGET}")
string(TOLOWER ${TARGET} TARGET)

add_subdirectory(src/ConfManager)
add_subdirectory(src/ExitManager)
add_subdirectory(src/FCoreLogger)
add_subdirectory(src/GIID-DB)
add_subdirectory(src/grpcServer)
add_subdirectory(src/grpcServices/Read)
add_subdirectory(src/grpcServices/Write)
add_subdirectory(src/Logger)
add_subdirectory(src/Main)
add_subdirectory(src/Plugins)
add_subdirectory(src/Services)
add_subdirectory(src/SignalManager)

list(
  APPEND EXTRA_LIBS
  ConfManager
  ExitManager
  FCoreLogger
  GIID_DB
  grpcServer
  grpcServiceRead
  grpcServiceWrite
  Logger
  Main
  Plugins
  Services
  SignalManager
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--disable-new-dtags")
message(STATUS "USER: CMAKE_EXE_LINKER_FLAGS = ${CMAKE_EXE_LINKER_FLAGS}")
add_executable(feel_core src/feel_core.cc)
target_link_libraries(feel_core PUBLIC ${EXTRA_LIBS})

if (CONAN)
  INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${PROJ_DIR}/bin/${TARGET}/base
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE)
endif()
