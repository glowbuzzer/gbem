cmake_minimum_required(VERSION 3.5)

project(etg C)

file(GLOB SOURCES "src/*.c")


add_library(etg STATIC ${SOURCES})
target_link_libraries(etg gberror)
target_include_directories(etg PUBLIC inc)