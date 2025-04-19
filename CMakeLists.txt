cmake_minimum_required(VERSION 3.30)
project(algorithm_analysis_homework1)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        algorithm_analysis_homework1
        src/main.cpp
        src/common.hpp
        src/utils.hpp
        src/utils_improved.hpp
        src/uhr/uhr_utils.hpp
        src/uhr/uhr.hpp
)
