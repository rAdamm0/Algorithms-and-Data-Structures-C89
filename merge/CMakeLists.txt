cmake_minimum_required(VERSION 3.10)
project(merge C)

# Set C standard to C89
set(CMAKE_C_STANDARD 89)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add executable
add_executable(merge merge-sort.c)