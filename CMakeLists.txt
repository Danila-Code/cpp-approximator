cmake_minimum_required(VERSION 3.16)
project(approximator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

# ======== approximator library ==============================
add_subdirectory(approximator_lib)

# ======== console interface =================================
add_subdirectory(app_console)

# ============================================================
# optional tests
# ============================================================
option(BUILD_TESTS "Build approximator tests" ON)
if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
