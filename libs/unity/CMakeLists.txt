cmake_minimum_required(VERSION 3.5)

project(unity C)

add_library(unity ./src/unity.c)

target_include_directories(unity PUBLIC inc)
#IMPORTANT NOTE - WE ARE SUPPRESSING COMPILER WARNINGS HERE
target_compile_options(unity PRIVATE -w)