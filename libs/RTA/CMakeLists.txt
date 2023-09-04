cmake_minimum_required(VERSION 3.5)

project(RTA C)

file(GLOB SOURCES "src/*.c")

add_library(RTA STATIC ${SOURCES})
target_link_libraries(RTA common-misc Logging machine_mapping EtherCAT)
target_include_directories(RTA PUBLIC inc)
