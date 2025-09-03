cmake_minimum_required(VERSION 3.20)

# Project name and language
project(rasterizer-cpp LANGUAGES CXX)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all source files automatically
file(GLOB SRC_FILES src/*.cpp)

# Create the executable
add_executable(rasterizer-cpp ${SRC_FILES})

# Include directories
target_include_directories(rasterizer-cpp PRIVATE include)
