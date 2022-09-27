cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

cpp_core_find_package(googletest VERSION v1.12.0 EXCLUDE_FROM_ALL)
find_package(Threads REQUIRED)

set(TESTS
  strand/base
  )

set(LIBRARIES
  strand
  GTest::gtest
  Threads::Threads)

include(../cmake/configure_tests.cmake)
configure_tests("coro" "${LIBRARIES}" ${TESTS})
