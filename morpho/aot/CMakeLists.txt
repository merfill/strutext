
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${FACTOR_BASE_SOURCE_DIR}/common)
include_directories(${FACTOR_BASE_SOURCE_DIR}/common/iterators)
include_directories(${FACTOR_BASE_SOURCE_DIR}/common/automata)
include_directories(${FACTOR_BASE_SOURCE_DIR}/texts/morpho/morpholib)
include_directories(${FACTOR_BASE_SOURCE_DIR}/texts/morpho/models)
include_directories(${FACTOR_BASE_SOURCE_DIR}/texts/symbols)

# aot dictionary parser.
set(NAME aot-parser)
add_executable(${NAME}
  main.cpp
)

target_link_libraries (${NAME}
  symbols
  morpho
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_SERIALIZATION_LIBRARY}
)
add_custom_command(OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rus.aot.bin
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/rus.aot.bin
    COMMAND /bin/ln
    ARGS ${CMAKE_CURRENT_SOURCE_DIR}/rus.aot.bin ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rus.aot.bin)
add_custom_target(aot ALL echo
    DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rus.aot.bin)    