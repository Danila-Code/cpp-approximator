find_package(GTest REQUIRED)

add_executable(tests src/tests.cpp)

target_link_libraries(tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    approximator_lib
)

add_test(NAME test_approximator COMMAND test_approximator)
