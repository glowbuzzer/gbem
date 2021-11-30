cmake_minimum_required(VERSION 3.5)

project(cannon_automata C)

file(GLOB SOURCES "src/*.c")

add_library(cannon_automata STATIC ${SOURCES})
target_link_libraries(cannon_automata common-misc Logging ec_mapping EtherCAT)
target_include_directories(cannon_automata PUBLIC inc)