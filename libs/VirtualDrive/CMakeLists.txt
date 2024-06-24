cmake_minimum_required(VERSION 3.5)

project(virtual_drive C)

file(GLOB SOURCES "src/*.c")

add_library(virtual_drive STATIC ${SOURCES})
target_link_libraries(virtual_drive common-misc Logging machine_mapping EtherCAT)
target_include_directories(virtual_drive PUBLIC inc ${CMAKE_SOURCE_DIR}/inc "${CMAKE_SOURCE_DIR}/shared/inc")