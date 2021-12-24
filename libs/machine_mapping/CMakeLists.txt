cmake_minimum_required(VERSION 3.5)

project(machine_mapping C)

file(GLOB SOURCES "src/*.c" "src/machines/*.c")

add_library(machine_mapping STATIC ${SOURCES})
target_link_libraries(machine_mapping common-misc Logging oriental_motor beckhoff cannon_automata delta cia kollmorgen dpm gberror EtherCAT JVL omron nanotec)
target_include_directories(machine_mapping PUBLIC inc inc/machines)