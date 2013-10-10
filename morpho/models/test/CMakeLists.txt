
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${FACTOR_BASE_SOURCE_DIR}/texts/morpho/models)

set(UNIT_TEST_NAME rus-morpho-model-unit-test)
set(UNIT_TEST_SOURCES
  test.cpp
)

add_executable(${UNIT_TEST_NAME} ${UNIT_TEST_SOURCES})

target_link_libraries(${UNIT_TEST_NAME}
  symbols
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)

add_boost_tests("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${UNIT_TEST_NAME}" "--catch_system_errors=no" ${UNIT_TEST_SOURCES})
