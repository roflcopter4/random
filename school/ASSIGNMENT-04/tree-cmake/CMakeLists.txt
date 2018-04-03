# Why am I bothering.

cmake_minimum_required(VERSION 3.00)
project(tree C)
include(FindPkgConfig)
include(CheckIncludeFile)

pkg_check_modules(TREE REQUIRED libbsd)
CHECK_INCLUDE_FILE(bsd/bsd.h HAS_BSD_H)

if( NOT HAS_BSD_H )
  message( FATAL_ERROR "Required header file bsd/bsd.h not found" )
endif()

add_definitions(-D_GNU_SOURCE -DVERSION="1.0.0")

add_subdirectory(src)
