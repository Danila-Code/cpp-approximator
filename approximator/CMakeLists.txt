cmake_minimum_required(VERSION 3.16)
project(approximator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

include_directories(
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
)

file(GLOB_RECURSE PROJECT_SOURCES
    "${INCLUDE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp"
)

# ===================== approximator_lib ====================
add_library(approximator_lib
    ${SOURCE_DIR}/approximator.cpp
    ${SOURCE_DIR}/equation_system.cpp
)

add_executable(
    approximator
    ${PROJECT_SOURCES}
)

target_include_directories(approximator PRIVATE
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
)

target_link_libraries(approximator PRIVATE approximator_lib)

# ============================================================
# optional tests
# ============================================================
option(BUILD_TESTS "Build approximator tests" ON)
if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
