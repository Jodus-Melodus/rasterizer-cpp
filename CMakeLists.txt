cmake_minimum_required(VERSION 3.20)
project(craster)

# List all source files that contain definitions
add_executable(craster
    src/main.cpp
)

# Include path for headers
target_include_directories(craster PRIVATE include)