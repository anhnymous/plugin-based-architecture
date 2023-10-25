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
project(feel-plugins VERSION 0.1.0 LANGUAGES C CXX DESCRIPTION "Feel Plugins")
message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
message(STATUS "PROJECT_VERSION: ${PROJECT_VERSION}")

include(CMakePrintHelpers)
cmake_print_variables(ENABLED_LANGUAGES)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
option(DEBUG "Build optimized for GDB debugging" ON)
if(DEBUG)
  message(STATUS "USER: feel-plugins debug build enabled")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb3 -DDEBUG -DVERBOSE")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb3 -DDEBUG -DVERBOSE")
else()
  message(STATUS "USER: feel-plugins release build enabled")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftree-vectorize")
endif()

set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfloat-equal")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wduplicated-cond")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat-security")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdouble-promotion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnull-dereference")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wlogical-not-parentheses")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE -Wextra -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -Wsign-conversion")

set(CODE_COVERAGE FALSE)
set(CMAKE_INSTALL_LIBDIR)
set(CMAKE_INSTALL_BINDIR)
set(CMAKE_INSTALL_SBINDIR)
set(CMAKE_INSTALL_INCLUDEDIR)
set(CMAKE_INSTALL_LIBEXECDIR)
set(CMAKE_INSTALL_DATAROOTDIR)
set(CMAKE_INSTALL_OLDINCLUDEDIR)
set(CMAKE_EXPORT_NO_PACKAGE_REGISTRY FALSE)

option(CONAN "Using Conan build system" OFF)
if (CONAN)
  set(CONAN_DISABLE_CHECK_COMPILER OFF)
  if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
    conan_define_targets()
  endif()
  get_filename_component(PROJ_DIR . ABSOLUTE)
endif()

include_directories(provider)
add_subdirectory(src/trivial/plugin-X)
add_subdirectory(src/trivial/plugin-Y)

if (CONAN)
  INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${PROJ_DIR}/bin/${TARGET}/base
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE)
endif()
