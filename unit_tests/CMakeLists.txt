# CppMath unit tests
#
# Created by Dmytro Krasnianskyi on 11.11.20.

cmake_minimum_required(VERSION 3.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

ADD_EXECUTABLE( test_cppmath_matrix cppmath_matrix_test.cpp )
add_test(NAME cppmath_matrix COMMAND test_cppmath_matrix)