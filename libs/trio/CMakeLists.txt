cmake_minimum_required(VERSION 3.5)

project(trio C)

file(GLOB SOURCES "src/*.c")

add_library(trio STATIC ${SOURCES})
target_link_libraries(trio common-misc Logging machine_mapping EtherCAT)
target_include_directories(trio PUBLIC inc)