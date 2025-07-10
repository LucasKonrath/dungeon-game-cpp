cmake_minimum_required(VERSION 3.10)
project(DungeonGameTests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable for simple tests
add_executable(simple_tests simple_tests.cpp)

# Add executable for main game
add_executable(dungeon_game dungeon-game.cpp)
