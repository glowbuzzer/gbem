cmake_minimum_required(VERSION 3.5)

project(automationware C)

#file(GLOB SOURCES "src/*.c")


#add_library(automationware STATIC ${SOURCES})
add_library(automationware STATIC src/aw_j_series.c src/aw_j_series_fsoe.c src/aw_j_series_misc.c)


target_link_libraries(automationware common-misc Logging machine_mapping EtherCAT sdos linux-shm dpm state_machine)
#target_include_directories(automationware PUBLIC inc)

target_include_directories(automationware PUBLIC inc ${CMAKE_SOURCE_DIR}/inc "${CMAKE_SOURCE_DIR}/shared/inc")
