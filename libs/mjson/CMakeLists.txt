cmake_minimum_required(VERSION 3.5)

project(mjson C)

file(GLOB SOURCES "src/*.c")


add_library(mjson STATIC ${SOURCES})

target_include_directories(mjson PUBLIC inc)