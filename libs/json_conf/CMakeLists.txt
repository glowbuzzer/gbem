cmake_minimum_required(VERSION 3.18)
project(json_conf C)

set(CMAKE_C_STANDARD 11)

find_package(OpenSSL REQUIRED)

file(GLOB SOURCES "src/*.c")

add_library(json_conf STATIC ${SOURCES})

target_link_libraries(json_conf jansson EtherCAT machine_mapping OpenSSL::SSL OpenSSL::Crypto)
target_include_directories(json_conf PUBLIC inc)

message(STATUS "Copying gbem_config.json to ${CMAKE_BINARY_DIR}/ from ${CMAKE_CURRENT_SOURCE_DIR}")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/gbem_config.json
        DESTINATION ${CMAKE_BINARY_DIR}/)

# Check if the file was copied
if (EXISTS "${CMAKE_BINARY_DIR}/gbem_config.json")
    message(STATUS "Copy succeeded.")
else ()
    message(WARNING "Copy failed.")
endif ()

