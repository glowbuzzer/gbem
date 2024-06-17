cmake_minimum_required(VERSION 3.5)

project(oriental_motor C)

file(GLOB SOURCES "src/*.c")

add_library(oriental_motor STATIC ${SOURCES})
target_link_libraries(oriental_motor common-misc Logging machine_mapping EtherCAT sdos linux-shm dpm state_machine)
target_include_directories(oriental_motor PUBLIC inc "${CMAKE_SOURCE_DIR}/inc" "${CMAKE_SOURCE_DIR}/shared/inc")