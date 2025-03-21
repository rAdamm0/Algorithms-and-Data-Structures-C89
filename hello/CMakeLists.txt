cmake_minimum_required(VERSION 3.10)
project(hello C)

# Set C standard to C89
set(CMAKE_C_STANDARD 89)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add executable
add_executable(hello hello.h hello.c hello-main.c)