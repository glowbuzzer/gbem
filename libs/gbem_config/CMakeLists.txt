cmake_minimum_required(VERSION 3.5)

project(gbem_config C)

file(GLOB SOURCES "src/*.c")

add_library(gbem_config INTERFACE)
target_include_directories(gbem_config INTERFACE inc ${gbem_config_BINARY_DIR})