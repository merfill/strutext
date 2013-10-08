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

# Search python interpretator.
message(STATUS "Searching for Python interpretator...")
find_package(PythonInterp 2.7 REQUIRED)
if(PYTHONINTERP_FOUND)
    message(STATUS "  Python interpreter found: " ${PYTHON_EXECUTABLE})
    message(STATUS "  Python version: " ${PYTHON_VERSION_STRING})
else(PYTHONINTERP_FOUND)
    message(FATAL_ERROR "Python interpreter not found. Install python package.")
endif(PYTHONINTERP_FOUND)

# Generate symbol table by using Python script.
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/symbol_table.cpp
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/st_generator.py -f ${CMAKE_CURRENT_SOURCE_DIR}/UnicodeData.txt -t ${CMAKE_CURRENT_BINARY_DIR}/symbol_table.cpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/UnicodeData.txt ${CMAKE_CURRENT_SOURCE_DIR}/st_generator.py
    COMMENT "Generating UNICODE symbol table"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# symbols library.
set(NAME symbols)
add_library(${NAME} STATIC
  ${CMAKE_CURRENT_BINARY_DIR}/symbol_table.cpp
)

add_subdirectory(test)
