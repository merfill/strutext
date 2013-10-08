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
# \brief Integration of Boost Unit Test library in CTest.
# \warning Boost test case name must be following format: NameOfModule_NameOfSubmodule_NameOfTest.
#
# \param EXECUTABLE Name of the UT executable.
# \param EXTRA_ARGS Extra arguments if any.
# \param SOURCES    Source file list where BOOST unit tests are located.

function (add_boost_tests EXECUTABLE EXTRA_ARGS)
  if (NOT ARGN)
    message(FATAL_ERROR "No source files given to `add_boost_tests'")
  endif()

  # Search BOOST_AUTO_TEST_CASE or BOOST_FIXTURE_TEST_CASE in the source files and add it as CTest test.
  foreach(source ${ARGN})
    file(STRINGS "${source}" found_tests REGEX "BOOST_(AUTO|FIXTURE)_TEST_CASE")
    foreach(hit ${found_tests})
        string(REGEX MATCH ".*\\(([a-zA-Z0-9]+_[a-zA-Z0-9]+_[a-zA-z0-9]+)(,|\\)).*" found_name ${hit})
        if (CMAKE_MATCH_1)
          message(STATUS "Adding Unit Test: ${CMAKE_MATCH_1}")
            add_test("${CMAKE_MATCH_1}" ${EXECUTABLE} --run_test=${CMAKE_MATCH_1} ${EXTRA_ARGS})
        else()
            string(REGEX MATCH ".*\\(([a-zA-Z0-9_]+)(,|\\)).*" incorrect_proc ${hit})
            message(WARNING "Test procedure name '${CMAKE_MATCH_1}' at '${source}' does not match naming policy")
        endif()
    endforeach()
  endforeach()
endfunction(add_boost_tests)
