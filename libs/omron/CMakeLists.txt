cmake_minimum_required(VERSION 3.5)

project(omron C)

file(GLOB SOURCES "src/*.c")

add_library(omron STATIC ${SOURCES})
target_link_libraries(omron common-misc Logging machine_mapping EtherCAT)
target_include_directories(omron PUBLIC inc)
