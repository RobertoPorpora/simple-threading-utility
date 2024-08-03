cmake_minimum_required(VERSION 3.10)

project(simple_threading_utility)

# Add the main library
add_library(STHR STATIC STHR.c)

# Include directories
target_include_directories(STHR PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})