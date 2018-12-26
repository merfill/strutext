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

cmake_minimum_required(VERSION 2.8.3)
project(strutext CXX)

message(STATUS "***** Strutext project *****")

set(STRUTEXT_ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(STRUTEXT_ROOT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# The path to the directory where cmake modules are.
set(CMAKE_MODULE_PATH ${STRUTEXT_ROOT_SOURCE_DIR}/cmake)

# Connection assembly configuration.
include(Build)

# Set depending on the boost.

# Compose boost libraries as static.
set(Boost_USE_STATIC_LIBS ON)

# Use the multithreaded version.
set(Boost_USE_MULTITHREADED ON)

# Does not replace the standard library.
set(Boost_USE_STATIC_RUNTIME OFF)

message(STATUS "Searching for Boost...")
find_package(Boost 1.53 COMPONENTS
  date_time
  filesystem
  program_options
  system
  unit_test_framework
  iostreams
  regex
  serialization
)

message(STATUS "Boost components summary:")
message(STATUS "  date time: ${Boost_DATE_TIME_FOUND}")
message(STATUS "  filesystem: ${Boost_FILESYSTEM_FOUND}")
message(STATUS "  program options: ${Boost_PROGRAM_OPTIONS_FOUND}")
message(STATUS "  system: ${Boost_SYSTEM_FOUND}")
message(STATUS "  unit test framework: ${Boost_UNIT_TEST_FRAMEWORK_FOUND}")
message(STATUS "  iostreams: ${Boost_IOSTREAMS_FOUND}")
message(STATUS "  serialization: ${Boost_SERIALIZATION_FOUND}")
message(STATUS "  regex: ${Boost_REGEX_FOUND}")

if (Boost_FOUND)
  message(STATUS "Boost library dirs: " ${Boost_LIBRARY_DIRS})
  message(STATUS "Boost include dirs: " ${Boost_INCLUDE_DIRS})
  include_directories(${Boost_INCLUDE_DIRS})
else ()
  message(FATAL_ERROR "Some boost libraries not found.")
endif ()

message(STATUS "Searching for Log4cplus...")
find_package(Log4cplus)
if (Log4cplus_FOUND)
  message(STATUS "Log4cplus library libs: " ${Log4cplus_LIBRARY})
  message(STATUS "Log4cplus include dir: " ${Log4cplus_INCLUDE_DIR})
  include_directories(${Log4cplus_INCLUDE_DIRS})
else ()
  message(FATAL_ERROR "Log4cplus not found in the system. On Ubuntu install package liblog4cplus-dev")
endif ()

# Define unit test data directory path.
set(STRUTEXT_UNIT_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ut-data")
add_definitions(-DSTRUTEXT_UNIT_TEST_DIR="${STRUTEXT_UNIT_TEST_DIR}")

# Putting it in the directories from the list below:
add_subdirectory(symbols)
add_subdirectory(encode)
add_subdirectory(automata)
add_subdirectory(morpho)
add_subdirectory(utility/test)
add_subdirectory(examples)
