cmake_minimum_required(VERSION 3.5)

project(beckhoff C)

file(GLOB SOURCES "src/*.c")

add_library(beckhoff STATIC ${SOURCES})
target_link_libraries(beckhoff common-misc Logging machine_mapping EtherCAT gberror ecm_status sdos linux-shm dpm state_machine)
target_include_directories(beckhoff PUBLIC inc "${CMAKE_SOURCE_DIR}/inc" "${CMAKE_SOURCE_DIR}/shared/inc")