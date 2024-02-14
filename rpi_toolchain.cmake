#SET(CMAKE_SYSTEM_NAME Generic)
#SET(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

#set(CMAKE_INSTALL_RPATH "/home/idein/x-tools/armv6-rpi-linux-gnueabihf/armv6-rpi-linux-gnueabihf/sysroot/lib")
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# specify the cross compiler
SET(CMAKE_C_COMPILER /home/idein/x-tools/armv6-rpi-linux-gnueabihf/bin/armv6-rpi-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER /home/idein/x-tools/armv6-rpi-linux-gnueabihf/bin/armv6-rpi-linux-gnueabihf-g++)

# specify the sysroot
SET(CMAKE_SYSROOT /home/idein/x-tools/armv6-rpi-linux-gnueabihf/armv6-rpi-linux-gnueabihf/sysroot)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# tell cmake not to try test compile
#SET(CMAKE_C_COMPILER_WORKS 1)
#SET(CMAKE_CXX_COMPILER_WORKS 1)
#SET(COMMON_FLAGS
#        "-mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb -mthumb-interwork -ffunction-sections -fdata-sections \
#    -g -fno-common -fmessage-length=0 -specs=nosys.specs -specs=nano.specs")
#SET(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++11")
#SET(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} ${DEFINES} -std=gnu11")
