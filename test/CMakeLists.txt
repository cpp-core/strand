cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

find_package(Threads REQUIRED)

set(TESTS
  strand/base
  )

set(LIBRARIES
  strand
  GTest::gtest
  Threads::Threads)

configure_tests("coro" "${LIBRARIES}" ${TESTS})
