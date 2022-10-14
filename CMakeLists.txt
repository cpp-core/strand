cmake_minimum_required (VERSION 3.24 FATAL_ERROR)
project (strand VERSION 0.1 LANGUAGES C CXX)

# Make including this file idempotent
#
if(TARGET strand OR TARGET strand::strand)
  return()
endif()

# If this is the current project.
#
if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  # Laod the cmake recipes and utilities.
  #
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/load_cmake_helpers.cmake)

  # Options for generating tests and documentation.
  #
  option(STRAND_TEST "Generate the tests." ON)
  option(STRAND_DOCS "Generate the docs." OFF)

  # compile_commands.json
  #
  symlink_compile_commands()

  # Convenience targets for git operations
  #
  make_submodule_pull_target()

else()
  option(STRAND_TEST "Generate the tests." OFF)
  option(STRAND_DOCS "Generate the docs." OFF)
endif()

# Put executables in the top-level binary directory
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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
add_chrono()

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
if(STRAND_TEST)
  add_gtest()
  add_subdirectory(test)
endif()

# Optionally configure the documentation
#
# if(STRAND_DOCS)
#   add_subdirectory(docs)
# endif()

# Installation
#
install_config(strand)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/coro/strand DESTINATION include/coro)
install(FILES ${PROJECT_SOURCE_DIR}/include/coro/consumer.h DESTINATION include/coro)
install(FILES ${PROJECT_SOURCE_DIR}/include/coro/transformer.h DESTINATION include/coro)
