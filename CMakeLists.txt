cmake_minimum_required(VERSION 3.14)    # or the version you have installed
project(test-gmock-2)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

# Get GoogleTest (that also contains GoogleMock) from Github using the FetchContent CMake module
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Compile our Example Library
include_directories(
    include
)
add_library(AtmMachine STATIC 
    src/AtmMachine.cpp
)

# Prepare things to test it and run tests including our testing folder "test"
enable_testing()
add_subdirectory(test)