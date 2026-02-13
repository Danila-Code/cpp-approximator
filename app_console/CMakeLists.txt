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

# ===================== console interface ====================
add_executable(app_console
    ${PROJECT_SOURCES}
)

target_link_libraries(app_console PRIVATE
    approximator_lib
)

target_include_directories(app_console PRIVATE
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../approximator_lib/include
)
