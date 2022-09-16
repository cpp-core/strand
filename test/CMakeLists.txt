cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

find_package(googletest REQUIRED)
find_package(Threads REQUIRED)

set(TESTS
  strand/base
  )

set(LIBRARIES
  cxx_coro_strand
  fmt::fmt
  googletest::cxx-googletest
  Threads::Threads)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/configure_tests.cmake)
configure_tests("coro" "${LIBRARIES}" ${TESTS})
