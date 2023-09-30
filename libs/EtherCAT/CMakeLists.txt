cmake_minimum_required(VERSION 3.5)

project(EtherCAT C)

file(GLOB SOURCES "SOEM/*.c" "os_linux/osal/*.c" "os_linux/oshw/*.c")
set(OS_LIBS pthread rt)
set(OS "linux")

add_library(EtherCAT STATIC ${SOURCES})
target_link_libraries(EtherCAT common-misc Logging pthread)
target_include_directories(EtherCAT PUBLIC SOEM os_linux/osal os_linux/oshw)
#IMPORRTANT NOTE - WE ARE SUPRESSING COMPILER WARNINGS HERE
target_compile_options(EtherCAT PRIVATE -w)

