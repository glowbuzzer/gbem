cmake_minimum_required(VERSION 3.5)

project(automationware C)

file(GLOB SOURCES "src/*.c")

add_library(automationware STATIC ${SOURCES})
target_link_libraries(automationware common-misc Logging machine_mapping EtherCAT)
target_include_directories(automationware PUBLIC inc)