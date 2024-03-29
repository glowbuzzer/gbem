
#this is the process name we try and find to send signals to
SET(GBC_PROCESS_NAME "GBC")


#defines to enable variable forcing through debugger
SET(CTRL_ENABLE_FORCING 0)
SET(PLC_ENABLE_FORCING 0)


SET(GBC_SHARED_MEMORY_NAME "gbc_shared_memory")


#Enables debug logging overall
add_compile_definitions(ENABLE_LOGGING=1)

#Sets the debug log level
#LOG_LVL_FATAL = 0
#LOG_LVL_ERROR = 1
#LOG_LVL_WARNING =  2
#LOG_LVL_INFO = 3
#LOG_LVL_DEBUG = 4
#LOG_LVL_TRACE = 5

add_compile_definitions(LOG_LEVEL=5)

#Enables user messages (standard messages that describe what is going on in GBEM output to console, log, syslog etc.)
add_compile_definitions(GBEM_UM_EN=1)

#Enables State Machine (SM) logging for GBEM (this produces voluminous output)
add_compile_definitions(GBEM_SM_LOG_EN=0)

#Enables general GBEM debig logging (general logging statements in key places)
add_compile_definitions(GBEM_GEN_LOG_EN=1)

#Enables missing function pointer logging for GBEM
add_compile_definitions(GBEM_MISSING_FUN_LOG_EN=0)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(DEBUG_BUILD=0)
else ()
    add_compile_definitions(DEBUG_BUILD=1)
endif ()

