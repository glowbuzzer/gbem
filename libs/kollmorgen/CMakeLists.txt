cmake_minimum_required(VERSION 3.5)

project(kollmorgen C)

file(GLOB SOURCES "src/*.c")

add_library(kollmorgen STATIC ${SOURCES})
target_link_libraries(kollmorgen common-misc Logging machine_mapping EtherCAT)
target_include_directories(kollmorgen PUBLIC inc ${CMAKE_SOURCE_DIR}/inc "${CMAKE_SOURCE_DIR}/shared/inc")
