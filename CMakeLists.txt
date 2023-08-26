# stop if cmake version below 3.12
cmake_minimum_required(VERSION 3.12)

macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach ()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

project(GBEM DESCRIPTION "Glowbuzzer EtherCAT Master" VERSION "0.0.1" LANGUAGES C)
message(STATUS "GB: We are building [${PROJECT_NAME}]")
set(OS_LIBS pthread rt)

file(GLOB SOURCES "src/*.c" "shared/src/*.c" "plc_user_code/*.c")


message(STATUS "GB: Using cmake version [${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}]")

#PRIVATE = Needed by me, but not by my dependents
#PUBLIC = Needed both by me and my dependents
#INTERFACE = Not needed by me, but my dependents


#print_all_variables()

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "GB: No build type was provided, we will default to [Release]")
    set(CMAKE_BUILD_TYPE "Release")
else ()
    message(STATUS "GB: CMAKE_BUILD_TYPE is [${CMAKE_BUILD_TYPE}]")
endif ()


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wsign-conversion -Og -fPIC -std=gnu99")


set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

message(STATUS "GB: CMAKE_C_FLAGS is ${CMAKE_C_FLAGS}")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "GB: CMAKE_C_FLAGS_DEBUG is ${CMAKE_C_FLAGS_DEBUG}")
endif ()
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "GB: CMAKE_C_FLAGS_RELEASE is ${CMAKE_C_FLAGS_RELEASE}")
endif ()


#If a GIT_TAG string is passed in (-D) we will add this to the version.h file. If no tag or empty string set the GIT_TAG to "dev_ver"
#if (NOT DEFINED GIT_TAG)
#    message(STATUS "GB: No Git Tag was passed to cmake. It will default to [dev_ver]")
#    set(GIT_TAG "dev_ver")
#elseif (GIT_TAG STREQUAL "")
#    message(STATUS "GB: Git Tag was empty. It will default to [dev_ver]")
#    set(GIT_TAG "dev_ver")
#else ()
#    message(STATUS "GB: The Git Tag was [${GIT_TAG}]")
#endif ()

file(READ "version.txt" GIT_TAG_TMP)

string(STRIP ${GIT_TAG_TMP} GIT_TAG)

message(STATUS "GB: GIT_TAG from version.txt is ${GIT_TAG}")

configure_file(version/version.h.in version/version.h)


#FLAVOUR is the platform variant - currently only PI or LINUX
if (NOT DEFINED FLAVOUR)
    message(STATUS "GB: No -DFLAVOUR=\"blah\" was provided we will default to [LINUX]")
    set(FLAVOUR "LINUX")
elseif (FLAVOUR STREQUAL "PI")
    set(FLAVOUR "PI")
else ()
    message(STATUS "GB: We could not match the FLAVOUR you provided. We will default to [LINUX]")
    set(FLAVOUR "LINUX")
endif ()

message(STATUS "GB: PROJECT_BINARY_DIR [${PROJECT_BINARY_DIR} ]")
message(STATUS "GB: CURRENT_SOURCE_DIR [${CMAKE_CURRENT_SOURCE_DIR} ]")


add_executable(GBEM ${SOURCES})

target_link_libraries(GBEM EtherCAT Logging mjson common-misc cia state_machine dpm ecm_status minini machine_mapping linux-shm pid sleep beckhoff cannon_automata kollmorgen automationware virtual_drive delta JVL omron oriental_motor unity gberror gbem_config etg m rt)
target_include_directories(GBEM PUBLIC "inc" "shared/inc" "plc_user_code" ${PROJECT_BINARY_DIR}/version ${gbem_config_BINARY_DIR})

add_executable(GBEM_testStateMachine shared/src/control.c shared/src/ec_functions.c src/ecrxtx.c src/ethercatnetscan.c src/status.c shared/src/json-maker.c shared/src/float_int_conv.c test/src/test.c test/src/testStateMachine.c shared/src/plc_core.c plc_user_code/plc_init.c src/cyclic_message.c plc_user_code/plc_task_1.c shared/src/iomap_processing.c)
target_link_libraries(GBEM_testStateMachine mjson EtherCAT gbem_config machine_mapping Logging common-misc cia state_machine dpm minini linux-shm pid sleep beckhoff delta kollmorgen oriental_motor unity gberror m)
target_include_directories(GBEM_testStateMachine PUBLIC "inc" "shared/inc" "test/inc" "plc_user_code")

add_executable(GBEM_testMisc shared/src/float_int_conv.c shared/src/control.c shared/src/ec_functions.c src/ecrxtx.c src/ethercatnetscan.c src/json_print.c src/status.c shared/src/json-maker.c test/src/test.c test/src/testMisc.c test/src/misc/etg1020_diag_test.c test/src/misc/drive_error_test.c test/src/misc/cia402_test.c test/src/misc/ethercat_test.c shared/src/plc_core.c plc_user_code/plc_init.c src/cyclic_message.c shared/src/iomap_processing.c plc_user_code/plc_task_1.c)
target_link_libraries(GBEM_testMisc mjson EtherCAT gbem_config machine_mapping Logging common-misc cia state_machine dpm minini linux-shm pid sleep beckhoff kollmorgen oriental_motor unity gberror m)
target_include_directories(GBEM_testMisc PUBLIC "inc" "shared/inc" "test/inc" "test/inc/misc" "plc_user_code")

add_executable(GBEM_testEthercat shared/src/control.c shared/src/ec_functions.c src/ethercatnetscan.c src/status.c shared/src/json-maker.c shared/src/float_int_conv.c test/src/test.c test/src/testEthercat.c src/ecrxtx.c src/cyclic_message.c shared/src/plc_core.c plc_user_code/plc_init.c shared/src/iomap_processing.c plc_user_code/plc_task_1.c)
target_link_libraries(GBEM_testEthercat mjson EtherCAT gbem_config machine_mapping Logging common-misc cia state_machine dpm minini linux-shm pid sleep beckhoff kollmorgen oriental_motor unity gberror m)
target_include_directories(GBEM_testEthercat PUBLIC "inc" "shared/inc" "test/inc" "plc_user_code")

add_executable(GBEM_testIo shared/src/control.c shared/src/ec_functions.c src/ecrxtx.c shared/src/float_int_conv.c shared/src/json-maker.c src/status.c src/ethercatnetscan.c test/src/test.c shared/src/plc_core.c plc_user_code/plc_init.c test/src/testIo.c src/cyclic_message.c plc_user_code/plc_task_1.c shared/src/iomap_processing.c)
target_link_libraries(GBEM_testIo mjson EtherCAT gbem_config Logging common-misc cia state_machine dpm minini machine_mapping linux-shm pid sleep beckhoff kollmorgen oriental_motor unity gberror m)
target_include_directories(GBEM_testIo PUBLIC "inc" "shared/inc" "test/inc" "plc_user_code")

add_executable(GBEM_testPLC shared/src/plc_core.c shared/src/plc_stdlib.c test/src/testPLC.c shared/src/float_int_conv.c plc_user_code/plc_init.c plc_user_code/plc_task_1.c test/src/test.c shared/src/control.c src/cyclic_message.c src/ecrxtx.c shared/src/json-maker.c shared/src/ec_functions.c src/status.c src/ethercatnetscan.c shared/src/iomap_processing.c)
target_link_libraries(GBEM_testPLC mjson EtherCAT gbem_config machine_mapping Logging common-misc cia state_machine dpm minini linux-shm pid sleep beckhoff kollmorgen oriental_motor unity gberror m)
target_include_directories(GBEM_testPLC PUBLIC "inc" "shared/inc" "test/inc" "plc_user_code/")

# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" OFF)

if (BUILD_DOC)
    # check if Doxygen is installed
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target(GBEM_doc_doxygen ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen"
                VERBATIM)
    else (DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
endif (BUILD_DOC)
unset(BUILD_DOC CACHE)


add_compile_definitions(GB_APP_LINUX)
MESSAGE(STATUS "GB: [TARGET_GBEM_LINUX] is being used")
if (FLAVOUR STREQUAL "PI")
    MESSAGE(STATUS "GB: Loading [gbem_defs_pi.cmake]")
    include(gbem_defs_pi.cmake)
    if (EXISTS gbem_custom_defs_pi.cmake)
        MESSAGE(STATUS "GB: Loading [gbem_custom_defs_pi.cmake]")
        include(gbem_custom_defs_pi.cmake)
    else ()
        MESSAGE(STATUS "GB: [gbem_custom_def_pi.cmake] does NOT exist (no overrides for standard definitions will be used)")
    endif ()
else ()
    MESSAGE(STATUS "GB: Loading [gbem_defs_linux.cmake]")
    include(gbem_defs_linux.cmake)
    if (EXISTS gbem_custom_defs_linux.cmake)
        MESSAGE(STATUS "GB: Loading [gbem_custom_defs_linuxc.make]")
        include(gbem_custom_defs_linux.cmake)
    else ()
        MESSAGE(STATUS "GB: gbem_custom_defs_linux.cmake does NOT exist (no overrides for standard definitions will be used)")
    endif ()

endif ()
configure_file(gbem_config_autogen.h.in libs/gbem_config/gbem_config_autogen.h)

add_subdirectory("libs/gbem_config")
add_subdirectory("libs/EtherCAT")
add_subdirectory("libs/oriental_motor")
add_subdirectory("libs/machine_mapping")
add_subdirectory("libs/beckhoff")
add_subdirectory("libs/kollmorgen")
add_subdirectory("libs/unity")
add_subdirectory("libs/mjson")
add_subdirectory("libs/ETG")
add_subdirectory("libs/JVL")
add_subdirectory("libs/omron")
add_subdirectory("libs/nanotec")
add_subdirectory("libs/ecm_status")
add_subdirectory("libs/cannon_automata")
add_subdirectory("libs/invertek")
add_subdirectory("libs/delta")
add_subdirectory("libs/AutomationWare")
add_subdirectory("libs/VirtualDrive")

#this has to to be placed after the definitions
#    add_subdirectory("apps/GBEM")


#print_all_variables()


#common
add_subdirectory("gclibs/common-misc")
add_subdirectory("gclibs/bcm2835")
add_subdirectory("gclibs/logging")
add_subdirectory("gclibs/pid")
add_subdirectory("gclibs/cia")
add_subdirectory("gclibs/dpm")
add_subdirectory("gclibs/linux-shm")
add_subdirectory("gclibs/sleep")
add_subdirectory("gclibs/gberror")
add_subdirectory("gclibs/minini")
add_subdirectory("gclibs/state_machine")


if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR “GB: In-source builds not allowed! Create a build directory and run CMake from there.”)
endif ()


install(TARGETS GBEM DESTINATION bin CONFIGURATIONS Release)