cmake_minimum_required(VERSION 3.5)

project(JVL C)

file(GLOB SOURCES "src/*.c")

add_library(JVL STATIC ${SOURCES})
target_link_libraries(JVL common-misc Logging machine_mapping EtherCAT)
target_include_directories(JVL PUBLIC inc)