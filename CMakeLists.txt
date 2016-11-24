cmake_minimum_required(VERSION 2.8)

project( cpp-si-units )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(units units.cpp)
set_target_properties(units PROPERTIES
    COMPILE_FLAGS "-std=c++11 -Wall -Werror")

