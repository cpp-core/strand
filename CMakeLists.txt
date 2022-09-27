cmake_minimum_required (VERSION 3.22 FATAL_ERROR)
project (strand VERSION 0.1 LANGUAGES C CXX)

# Make including this file idempotent
#
if(TARGET strand OR TARGET strand::strand)
  return()
endif()

# Configuration variables.
#
# Build the tests and set directories if this is the master project
#
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(CORO_STRAND_TEST ON CACHE BOOL "Generate the tests.")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_DOCS_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
  
  # Have cmake output the compilation commands and create a link to it
  # from the project root directory so the language server can find
  # it.
  #
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json)
endif()

# Options for generating tests and documentation.
#
option(STRAND_TEST "Generate the tests." OFF)
option(STRAND_DOCS "Generate the docs." OFF)

# Output configuration
#
message("-- strand: Configuration: ${CMAKE_CURRENT_SOURCE_DIR}")
message("-- strand: Included from: ${CMAKE_SOURCE_DIR}")
message("-- strand: STRAND_TEST ${STRAND_TEST}")
message("-- strand: STRAND_DOCS ${STRAND_DOCS}")
message("-- strand: CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")

# Setup compilation before adding dependencies
#
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall)
add_compile_options(-stdlib=libc++)
add_compile_options(-O)
add_link_options(-stdlib=libc++)

# Add our dependencies
#
include(cmake/cpp_core_find_package.cmake)
cpp_core_find_package(chrono)

# Build the library
#
set(SOURCES
  strand/real_scheduler
  strand/scheduler
  strand/virtual_scheduler
  strand/yield
  )

foreach(NAME ${SOURCES})
  list(APPEND FILES "src/coro/${NAME}.cpp")
endforeach()

add_library(strand ${FILES})
add_library(strand::strand ALIAS strand)
target_link_libraries(strand PUBLIC chrono::chrono)

# Define the header files for this library.
#
file(GLOB_RECURSE PUBLIC_INCLUDE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)
target_sources(strand PUBLIC FILE_SET HEADERS BASE_DIRS include FILES ${PUBLIC_INCLUDE_FILES})

# Optionally configure the tests
#
if(CORO_STRAND_TEST)
  enable_testing()
  add_subdirectory(test)
endif()

# Optionally configure the documentation
#
# if(CORO_STRAND_DOCS)
#   add_subdirectory(docs)
# endif()

# Installation
#
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/install_config.cmake)
install_config(strand)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/coro/strand DESTINATION include/coro)
install(FILES ${PROJECT_SOURCE_DIR}/include/coro/consumer.h DESTINATION include/coro)
install(FILES ${PROJECT_SOURCE_DIR}/include/coro/transformer.h DESTINATION include/coro)
