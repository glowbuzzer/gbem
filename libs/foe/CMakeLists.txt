cmake_minimum_required(VERSION 3.5)

project(foe C)

file(GLOB SOURCES "src/*.c")

add_library(foe STATIC ${SOURCES})

target_link_libraries(foe common-misc Logging gbem_config EtherCAT)
target_include_directories(foe PUBLIC inc ${CMAKE_SOURCE_DIR}/inc "${CMAKE_SOURCE_DIR}/shared/inc")