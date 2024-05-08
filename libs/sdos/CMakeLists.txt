cmake_minimum_required(VERSION 3.5)

project(sdos C)

file(GLOB SOURCES "src/*.c" "src/machines/*.c")

add_library(sdos STATIC ${SOURCES})

target_link_libraries(sdos common-misc Logging oriental_motor beckhoff gbem_config cannon_automata delta cia kollmorgen dpm gberror EtherCAT JVL omron bbh nanotec virtual_drive automationware trio RTA)
target_include_directories(sdos PUBLIC inc)