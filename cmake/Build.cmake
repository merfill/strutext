# Copyright &copy; 2013, Vladimir Lapshin.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
# \brief Build configuration.

# Do not allow in source build.
if ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

# Allowing only Debug and Release build types.
# Set Debug build type by default.
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
  message(STATUS "Build type is not set. Use Debug build type by default")
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${STRUTEXT_ROOT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${STRUTEXT_ROOT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${STRUTEXT_ROOT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)

# General Flags for GCC.
if (${CMAKE_COMPILER_IS_GNUCXX})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fPIC -Wall -Wno-error -D__STDC_LIMIT_MACROS=1 -I.")
endif (${CMAKE_COMPILER_IS_GNUCXX})

# Definitions for build types.
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  message(STATUS "Debug configuration")
  # Flags fog GCC.
  if (${CMAKE_COMPILER_IS_GNUCXX})
    # Profiling: run cmake -DGPROF=1.
    if (GPROF)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    endif (GPROF)

    # Do not allow function inlining in debug mode.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-inline-functions -fno-inline -O -ggdb3")
  endif (${CMAKE_COMPILER_IS_GNUCXX})
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
  message(STATUS "Release configuration")
  # Flags fog GCC.
  if (${CMAKE_COMPILER_IS_GNUCXX})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  endif (${CMAKE_COMPILER_IS_GNUCXX})
else (${CMAKE_BUILD_TYPE} STREQUAL "Release")
  message(FATAL_ERROR "Only Release and Debug build types are allowed")
endif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")

# Allow cmake testing.
enable_testing()
include(CTest)

# Tests are added by using add_boost_tests procedure.
include(AddBoostTests)
