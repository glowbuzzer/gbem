cmake_minimum_required(VERSION 3.5)

project(nanotec C)

file(GLOB SOURCES "src/*.c")

add_library(nanotec STATIC ${SOURCES})
target_link_libraries(nanotec common-misc Logging machine_mapping EtherCAT)
target_include_directories(nanotec PUBLIC inc)
