# CppMath
#
# Created by Dmytro Krasnianskyi on 11.11.20.

cmake_minimum_required(VERSION 3.0)

# project(CppMath VERSION 1.0)

#collect all sources from the project directory
file(GLOB COLECTED_SRC
    "*.h"
    "*.cpp"
    "*.hpp"
)

configure_file(cppmath_config.h.in "${PROJECT_BINARY_DIR}/cppmath_config.h")
# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(SOURCE_LIB cppmath cppmath_matrix cppmath_matrix_base.cpp cppmath_matrix_base.hpp)
add_library(CppMath STATIC "${COLECTED_SRC}")

# target_include_directories(CppMath PUBLIC "${PROJECT_BINARY_DIR}")
include_directories( BEFORE "${PROJECT_BINARY_DIR}")

