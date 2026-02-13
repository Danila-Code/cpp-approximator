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
    ${PROJECT_SOURCES}
)

#add_library(approximator_lib
    #${SOURCE_DIR}/approximator.cpp
    #${SOURCE_DIR}/equation_system.cpp
#)

target_include_directories(approximator_lib PRIVATE
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
)
