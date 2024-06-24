cmake_minimum_required(VERSION 3.5)

project(invertek C)

file(GLOB SOURCES "src/*.c")

add_library(invertek STATIC ${SOURCES})
target_link_libraries(invertek common-misc Logging machine_mapping EtherCAT)
target_include_directories(invertek PUBLIC inc ${CMAKE_SOURCE_DIR}/inc "${CMAKE_SOURCE_DIR}/shared/inc")