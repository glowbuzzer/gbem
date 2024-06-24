cmake_minimum_required(VERSION 3.5)

project(delta C)

file(GLOB SOURCES "src/*.c")

add_library(delta STATIC ${SOURCES})
target_link_libraries(delta common-misc Logging machine_mapping EtherCAT)
target_include_directories(delta PUBLIC inc ${CMAKE_SOURCE_DIR}/inc "${CMAKE_SOURCE_DIR}/shared/inc")