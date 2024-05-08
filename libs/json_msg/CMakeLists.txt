cmake_minimum_required(VERSION 3.5)

project(json_msg C)

file(GLOB SOURCES "src/*.c" "src/machines/*.c")

add_library(json_msg STATIC ${SOURCES})

target_link_libraries(json_msg common-misc jansson Logging oriental_motor beckhoff gbem_config cannon_automata delta cia kollmorgen dpm gberror EtherCAT JVL omron bbh nanotec virtual_drive automationware trio RTA)
target_include_directories(json_msg PUBLIC inc)