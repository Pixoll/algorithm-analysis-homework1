cmake_minimum_required(VERSION 3.5)
project(algorithm_analysis_homework1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(
        algorithm_analysis_homework1
        src/main.cpp
        src/common.hpp
        src/utils.hpp
        src/utils_improved.hpp
        src/uhr/uhr_utils.hpp
        src/uhr/uhr.hpp
        src/brute_force/brute_force.hpp
        src/brute_force/brute_force_improved.hpp
        src/divide_and_conquer/divide_and_conquer.cpp
        src/divide_and_conquer/divide_and_conquer.hpp
        src/divide_and_conquer/divide_and_conquer_improved.cpp
        src/divide_and_conquer/divide_and_conquer_improved.hpp
)
