cmake_minimum_required (VERSION 3.1)
project(jansson C)

# Options
option(JANSSON_BUILD_SHARED_LIBS "Build shared libraries." OFF)
option(USE_URANDOM "Use /dev/urandom to seed the hash function." ON)
option(USE_WINDOWS_CRYPTOAPI "Use CryptGenRandom to seed the hash function." OFF)


option(JANSSON_EXAMPLES "Compile example applications" OFF)

if (UNIX)
   option(JANSSON_COVERAGE "(GCC Only! Requires gcov/lcov to be installed). Include target for doing coverage analysis for the test suite. Note that -DCMAKE_BUILD_TYPE=Debug must be set" OFF)
endif ()

# Set some nicer output dirs.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(JANSSON_TEMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/tmp)


# This is how I thought it should go
# set (JANSSON_VERSION "2.3.1")
# set (JANSSON_SOVERSION 2)

set(JANSSON_DISPLAY_VERSION "2.14")

# This is what is required to match the same numbers as automake's
set(JANSSON_VERSION "4.14.0")
set(JANSSON_SOVERSION 4)

# for CheckFunctionKeywords
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include (CheckCSourceCompiles)
include (CheckFunctionExists)
include (CheckFunctionKeywords)
include (CheckIncludeFiles)
include (CheckTypeSize)

# suppress format-truncation warning
include (CheckCCompilerFlag)
check_c_compiler_flag(-Wno-format-truncation HAS_NO_FORMAT_TRUNCATION)
if (HAS_NO_FORMAT_TRUNCATION)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-format-truncation")
endif()



message("C compiler: ${CMAKE_C_COMPILER_ID}")


check_include_files (endian.h HAVE_ENDIAN_H)
check_include_files (fcntl.h HAVE_FCNTL_H)
check_include_files (sched.h HAVE_SCHED_H)
check_include_files (unistd.h HAVE_UNISTD_H)
check_include_files (sys/param.h HAVE_SYS_PARAM_H)
check_include_files (sys/stat.h HAVE_SYS_STAT_H)
check_include_files (sys/time.h HAVE_SYS_TIME_H)
check_include_files (sys/types.h HAVE_SYS_TYPES_H)

check_function_exists (close HAVE_CLOSE)
check_function_exists (getpid HAVE_GETPID)
check_function_exists (gettimeofday HAVE_GETTIMEOFDAY)
check_function_exists (open HAVE_OPEN)
check_function_exists (read HAVE_READ)
check_function_exists (sched_yield HAVE_SCHED_YIELD)

# Check for the int-type includes
check_include_files (stdint.h HAVE_STDINT_H)

# Check our 64 bit integer sizes
check_type_size (__int64 __INT64)
check_type_size (int64_t INT64_T)
check_type_size ("long long" LONG_LONG_INT)

# Check our 32 bit integer sizes
check_type_size (int32_t INT32_T)
check_type_size (__int32 __INT32)
check_type_size ("long" LONG_INT)
check_type_size ("int" INT)
if (HAVE_INT32_T)
   set (JSON_INT32 int32_t)
elseif (HAVE___INT32)
   set (JSON_INT32 __int32)
elseif (HAVE_LONG_INT AND (LONG_INT EQUAL 4))
   set (JSON_INT32 long)
elseif (HAVE_INT AND (INT EQUAL 4))
   set (JSON_INT32 int)
else ()
   message (FATAL_ERROR "Could not detect a valid 32-bit integer type")
endif ()

check_type_size ("unsigned long" UNSIGNED_LONG_INT)
check_type_size ("unsigned int" UNSIGNED_INT)
check_type_size ("unsigned short" UNSIGNED_SHORT)

check_type_size (uint32_t UINT32_T)
check_type_size (__uint32 __UINT32)
if (HAVE_UINT32_T)
   set (JSON_UINT32 uint32_t)
elseif (HAVE___UINT32)
   set (JSON_UINT32 __uint32)
elseif (HAVE_UNSIGNED_LONG_INT AND (UNSIGNED_LONG_INT EQUAL 4))
   set (JSON_UINT32 "unsigned long")
elseif (HAVE_UNSIGNED_INT AND (UNSIGNED_INT EQUAL 4))
   set (JSON_UINT32 "unsigned int")
else ()
      message (FATAL_ERROR "Could not detect a valid unsigned 32-bit integer type")
endif ()

check_type_size (uint16_t UINT16_T)
check_type_size (__uint16 __UINT16)
if (HAVE_UINT16_T)
   set (JSON_UINT16 uint16_t)
elseif (HAVE___UINT16)
   set (JSON_UINT16 __uint16)
elseif (HAVE_UNSIGNED_INT AND (UNSIGNED_INT EQUAL 2))
   set (JSON_UINT16 "unsigned int")
elseif (HAVE_UNSIGNED_SHORT AND (UNSIGNED_SHORT EQUAL 2))
   set (JSON_UINT16 "unsigned short")
else ()
      message (FATAL_ERROR "Could not detect a valid unsigned 16-bit integer type")
endif ()




check_type_size (uint8_t UINT8_T)
check_type_size (__uint8 __UINT8)
if (HAVE_UINT8_T)
   set (JSON_UINT8 uint8_t)
elseif (HAVE___UINT8)
   set (JSON_UINT8 __uint8)
else ()
   set (JSON_UINT8 "unsigned char")
endif ()

# Check for ssize_t and SSIZE_T existence.
check_type_size(ssize_t SSIZE_T)
check_type_size(SSIZE_T UPPERCASE_SSIZE_T)
if(NOT HAVE_SSIZE_T)
   if(HAVE_UPPERCASE_SSIZE_T)
      set(JSON_SSIZE SSIZE_T)
   else()
      set(JSON_SSIZE int)
   endif()
endif()
set(CMAKE_EXTRA_INCLUDE_FILES "")

# Check for all the variants of strtoll
check_function_exists (strtoll HAVE_STRTOLL)
check_function_exists (strtoq HAVE_STRTOQ)
check_function_exists (_strtoi64 HAVE__STRTOI64)

# Figure out what variant we should use
if (HAVE_STRTOLL)
   set (JSON_STRTOINT strtoll)
   message(STATUS "HAVE_STRTOLL")
elseif (HAVE_STRTOQ)
   message(STATUS "HAVE_STRTOQ")
   set (JSON_STRTOINT strtoq)
elseif (HAVE__STRTOI64)
   message(STATUS "HAVE_STRTOI64")
   set (JSON_STRTOINT _strtoi64)
else ()
   message(STATUS "FALLBACK")
   # fallback to strtol (32 bit)
   # this will set all the required variables
   set (JSON_STRTOINT strtol)
   set (JSON_INT_T long)
   set (JSON_INTEGER_FORMAT "\"ld\"")
endif ()

# if we haven't defined JSON_INT_T, then we have a 64 bit conversion function.
# detect what to use for the 64 bit type.
# Note: I will prefer long long if I can get it, as that is what the automake system aimed for.
if (NOT DEFINED JSON_INT_T)
   if (HAVE_LONG_LONG_INT AND (LONG_LONG_INT EQUAL 8))
      set (JSON_INT_T "long long")
   elseif (HAVE_INT64_T)
      set (JSON_INT_T int64_t)
   elseif (HAVE___INT64)
      set (JSON_INT_T __int64)
   else ()
      message (FATAL_ERROR "Could not detect 64 bit type, although I detected the strtoll equivalent")
   endif ()

   set (JSON_INTEGER_FORMAT "\"lld\"")
endif ()


# If locale.h and localeconv() are available, define to 1, otherwise to 0.
check_include_files (locale.h HAVE_LOCALE_H)
check_function_exists (localeconv HAVE_LOCALECONV)

if (HAVE_LOCALECONV AND HAVE_LOCALE_H)
   set (JSON_HAVE_LOCALECONV 1)
else ()
   set (JSON_HAVE_LOCALECONV 0)
endif()

# check if we have setlocale
check_function_exists(setlocale HAVE_SETLOCALE)

# Check what the inline keyword is.
# Note that the original JSON_INLINE was always set to just 'inline', so this goes further.
check_function_keywords("inline")
check_function_keywords("__inline")
check_function_keywords("__inline__")

if (HAVE_INLINE)
   set(JSON_INLINE inline)
elseif (HAVE___INLINE)
   set(JSON_INLINE __inline)
elseif (HAVE___INLINE__)
   set(JSON_INLINE __inline__)
else()
   # no inline on this platform
   set (JSON_INLINE)
endif()

check_c_source_compiles ("int main() { unsigned long val; __sync_bool_compare_and_swap(&val, 0, 1); __sync_add_and_fetch(&val, 1); __sync_sub_and_fetch(&val, 1); return 0; } " HAVE_SYNC_BUILTINS)
check_c_source_compiles ("int main() { char l; unsigned long v; __atomic_test_and_set(&l, __ATOMIC_RELAXED); __atomic_store_n(&v, 1, __ATOMIC_RELEASE); __atomic_load_n(&v, __ATOMIC_ACQUIRE); __atomic_add_fetch(&v, 1, __ATOMIC_ACQUIRE); __atomic_sub_fetch(&v, 1, __ATOMIC_RELEASE); return 0; }" HAVE_ATOMIC_BUILTINS)

if (HAVE_SYNC_BUILTINS)
  set(JSON_HAVE_SYNC_BUILTINS 1)
else()
  set(JSON_HAVE_SYNC_BUILTINS 0)
endif()

if (HAVE_ATOMIC_BUILTINS)
  set(JSON_HAVE_ATOMIC_BUILTINS 1)
else()
  set(JSON_HAVE_ATOMIC_BUILTINS 0)
endif()

set (JANSSON_INITIAL_HASHTABLE_ORDER 3 CACHE STRING "Number of buckets new object hashtables contain is 2 raised to this power. The default is 3, so empty hashtables contain 2^3 = 8 buckets.")

# configure the public config file
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/cmake/jansson_config.h.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/include/jansson_config.h)

# Copy the jansson.h file to the public include folder
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/jansson.h
           DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/)

add_definitions(-DJANSSON_USING_CMAKE)

# configure the private config file
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/cmake/jansson_private_config.h.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/private_include/jansson_private_config.h)

# and tell the source code to include it
add_definitions(-DHAVE_CONFIG_H)

#include_directories (${CMAKE_CURRENT_BINARY_DIR}/include)
#include_directories (${CMAKE_CURRENT_BINARY_DIR}/private_include)

#message(STATUS ${CMAKE_CURRENT_BINARY_DIR}/include)

# Add the lib sources.
file(GLOB JANSSON_SRC src/*.c)

set(JANSSON_HDR_PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}/src/hashtable.h
   ${CMAKE_CURRENT_SOURCE_DIR}/src/jansson_private.h
   ${CMAKE_CURRENT_SOURCE_DIR}/src/strbuffer.h
   ${CMAKE_CURRENT_SOURCE_DIR}/src/utf.h
   ${CMAKE_CURRENT_BINARY_DIR}/private_include/jansson_private_config.h)

set(JANSSON_HDR_PUBLIC
   ${CMAKE_CURRENT_BINARY_DIR}/include/jansson_config.h
   ${CMAKE_CURRENT_SOURCE_DIR}/src/jansson.h)

source_group("Library Sources" FILES ${JANSSON_SRC})
source_group("Library Private Headers" FILES ${JANSSON_HDR_PRIVATE})
source_group("Library Public Headers" FILES ${JANSSON_HDR_PUBLIC})

   add_library(jansson STATIC
      ${JANSSON_SRC}
      ${JANSSON_HDR_PRIVATE}
      ${JANSSON_HDR_PUBLIC})
   set_target_properties(jansson PROPERTIES
      POSITION_INDEPENDENT_CODE true)


#target_include_directories(jansson PUBLIC src)


target_include_directories (jansson PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include)
target_include_directories (jansson PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/private_include)

#message(STATUS ${CMAKE_CURRENT_BINARY_DIR}/include)


message(STATUS "JSON_INT_T: ${JSON_INT_T}")
message (STATUS "JSON_INTEGER_FORMAT: ${JSON_INTEGER_FORMAT}")
