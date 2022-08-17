cmake_minimum_required(VERSION 3.14)    # or the version you have installed

# Include mock classes (that are in the mock folder)
include_directories(mock)

# The test program 1
add_executable(example_test_1 
    example_test_1.cpp
)
# Linking to the test program the GTest and GMock libraries and our Example lib.
target_link_libraries(example_test_1
    GTest::gtest_main 
    GTest::gmock
    AtmMachine
)

# The test program 2
add_executable(example_test_2 
    example_test_2.cpp
)
target_link_libraries(example_test_2
    GTest::gtest_main 
    GTest::gmock
    AtmMachine
)

# The test program 3
add_executable(example_test_3
    example_test_3.cpp
)
target_link_libraries(example_test_3
    GTest::gtest_main 
    GTest::gmock
    AtmMachine
)

# The test program 4
add_executable(example_test_4
    example_test_4.cpp
)
target_link_libraries(example_test_4
    GTest::gtest_main 
    GTest::gmock
    AtmMachine
)

# The test program 5
add_executable(example_test_5
    example_test_5.cpp
)
target_link_libraries(example_test_5
    GTest::gtest_main 
    GTest::gmock
    AtmMachine
)


# Include CMake module for GoogleTest and automatically add all the tests in our test program
include(GoogleTest)
gtest_discover_tests(example_test_1) # alternative: add_test(test1 example_test_1)
gtest_discover_tests(example_test_2)
gtest_discover_tests(example_test_3)
gtest_discover_tests(example_test_4)
gtest_discover_tests(example_test_5)