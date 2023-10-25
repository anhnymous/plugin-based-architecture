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
project(feel_feature VERSION 0.1.0 LANGUAGES C CXX DESCRIPTION "Feel Features")
message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
message(STATUS "PROJECT_VERSION: ${PROJECT_VERSION}")
include(common_cmake/common.cmake)

include(CMakePrintHelpers)
cmake_print_variables(ENABLED_LANGUAGES)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE OFF)

configure_file(${PROJECT_SOURCE_DIR}/src/FFeatureVersion.hh.in
               ${PROJECT_BINARY_DIR}/FFeatureVersion.hh)

set(FCore_SERVER_PORT 9997)
set(FCore_SERVER_HOST localhost)
configure_file(${PROJECT_SOURCE_DIR}/src/FFeatureConfig.hh.in
               ${PROJECT_BINARY_DIR}/FFeatureConfig.hh)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
option(DEBUG "Build FCore optimized for GDB debugging" OFF)
if(DEBUG)
  message(STATUS "USER: feel_feature debug build enabled")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb3 -DDEBUG -DVERBOSE")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb3 -DDEBUG -DVERBOSE")
else()
  message(STATUS "USER: feel_feature release build enabled")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftree-vectorize")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfloat-equal")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wduplicated-cond")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat-security")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdouble-promotion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wlogical-not-parentheses")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE -Wextra -Wall -Werror")

include(GNUInstallDirs)
set(CMAKE_INSTALL_LIBDIR)
set(CMAKE_INSTALL_BINDIR)
set(CMAKE_INSTALL_SBINDIR)
set(CMAKE_INSTALL_INCLUDEDIR)
set(CMAKE_INSTALL_LIBEXECDIR)
set(CMAKE_INSTALL_DATAROOTDIR)
set(CMAKE_INSTALL_OLDINCLUDEDIR)

set(CODE_COVERAGE FALSE)
set(POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_NO_PACKAGE_REGISTRY)
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

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
set(CONAN_LIBCXX)
set(CONAN_C_FLAGS)
set(CONAN_COMPILER)
set(CONAN_EXPORTED)
set(CONAN_CXX_FLAGS)
set(CONAN_CPU_COUNT 4)
set(CONAN_STD_CXX_FLAG)
set(CONAN_IN_LOCAL_CACHE)
set(CONAN_COMPILER_VERSION)
set(CONAN_CMAKE_CXX_STANDARD)
set(CONAN_SHARED_LINKER_FLAGS)
set(CONAN_CMAKE_CXX_EXTENSIONS)
set(CONAN_CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_definitions(-DTARGET="${TARGET}")
message(STATUS "USER: build target is ${TARGET}")
string(TOLOWER ${TARGET} TARGET)

add_subdirectory(src/grpcServices/Read)
add_subdirectory(src/grpcServices/Write)
add_subdirectory(src/Interfaces)
add_subdirectory(src/Main)
add_subdirectory(src/Plugins)
add_subdirectory(src/Services)

list(
  APPEND EXTRA_LIBS
  grpcServiceRead
  grpcServiceWrite
  Interfaces
  Main
  Plugins
  Services
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--disable-new-dtags")
add_executable(feel_feature src/feel_feature.cc)
target_link_libraries(feel_feature PUBLIC ${EXTRA_LIBS})

if (CONAN)
  INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${PROJ_DIR}/bin/${TARGET}/base
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE)
endif()
