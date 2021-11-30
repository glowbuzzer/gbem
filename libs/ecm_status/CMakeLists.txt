cmake_minimum_required(VERSION 3.5)

project(ecm_status C)

file(GLOB SOURCES "src/*.c")


add_library(ecm_status INTERFACE)

target_link_libraries(ecm_status INTERFACE gbem_config)
target_include_directories(ecm_status INTERFACE inc)