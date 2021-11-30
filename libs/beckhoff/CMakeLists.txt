cmake_minimum_required(VERSION 3.5)

project(beckhoff C)

file(GLOB SOURCES "src/*.c")

add_library(beckhoff STATIC ${SOURCES})
target_link_libraries(beckhoff common-misc Logging machine_mapping EtherCAT gberror ecm_status)
target_include_directories(beckhoff PUBLIC inc)