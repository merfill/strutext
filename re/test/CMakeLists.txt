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

include_directories(${Log4cplus_INCLUDE_DIR})
include_directories(${STRUTEXT_ROOT_SOURCE_DIR}/re)

set(UNIT_TEST_MODULE re-unit-test)
set(UNIT_TESTS_SOURCES
  ${STRUTEXT_ROOT_SOURCE_DIR}/ut-data/ut_main.cpp
  ansi_symbol_set_test.cpp
)

add_executable(${UNIT_TEST_MODULE} ${UNIT_TESTS_SOURCES})
target_link_libraries(${UNIT_TEST_MODULE}
  re
  ${Log4cplus_LIBRARY}
  ${Boost_REGEX_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_boost_tests("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${UNIT_TEST_MODULE}" "--catch_system_errors=no" ${UNIT_TESTS_SOURCES})
