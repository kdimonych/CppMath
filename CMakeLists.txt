# CppMath
#
# Created by Dmytro Krasnianskyi on 11.11.20.

cmake_minimum_required(VERSION 3.0)

PROJECT( CppMath LANGUAGES CXX VERSION 1.0 )

########################################################################
# Include directories
########################################################################
INCLUDE_DIRECTORIES(
#  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src
)

ADD_SUBDIRECTORY( src )
########################################################################
# Tests
########################################################################
ENABLE_TESTING()
ADD_SUBDIRECTORY( unit_tests )

