cmake_minimum_required(VERSION 3.14)    # or the version you have installed

# The test program
add_executable(example_test 
    example_test.cpp
)
# Linking to the test program the GTest libraries and our Example lib.
# Update: Including also gMock only to use the EXPECT_THAT assertion and Matchers
target_link_libraries(example_test 
    GTest::gtest_main 
    GTest::gmock
    Example
)

# Include CMake module for GoogleTest and automatically add all the tests in our test program
include(GoogleTest)
gtest_discover_tests(example_test) # alternative: add_test(test1 example_test)