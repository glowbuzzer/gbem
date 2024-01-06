cmake_minimum_required(VERSION 3.5)

project(bbh C)

file(GLOB SOURCES "src/*.c")

add_library(bbh STATIC ${SOURCES})
target_link_libraries(bbh common-misc Logging machine_mapping EtherCAT)
target_include_directories(bbh PUBLIC inc)