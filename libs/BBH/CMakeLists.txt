cmake_minimum_required(VERSION 3.5)

project(bbh C)

file(GLOB SOURCES "src/*.c")

add_library(bbh STATIC ${SOURCES})
target_link_libraries(bbh common-misc Logging gbem_config machine_mapping EtherCAT sdos linux-shm dpm state_machine)
target_include_directories(bbh PUBLIC inc "${CMAKE_SOURCE_DIR}/inc" "${CMAKE_SOURCE_DIR}/shared/inc")