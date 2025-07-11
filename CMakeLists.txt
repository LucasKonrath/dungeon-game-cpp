cmake_minimum_required(VERSION 3.10)
project(DungeonGameTests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")
endif()

# Add executable for simple tests
add_executable(simple_tests simple_tests.cpp)

# Add executable for callgraph generator
add_executable(callgraph_generator callgraph_generator.cpp)

# Add test
add_test(NAME unit_tests COMMAND simple_tests)
add_test(NAME callgraph_test COMMAND callgraph_generator)
