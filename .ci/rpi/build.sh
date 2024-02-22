#!/bin/sh -l

# configure trial version
#cat > /tmp/gbc_custom_defs_pi.cmake <<EOF
#  SET(TRIAL_VERSION_TIME_LIMIT_SECS 3660)
#  add_compile_definitions(TRIAL_VERSION=1)
#EOF

# Cross compile GBC executable for RPi
# (note that this runs as entrypoint inside local docker image based on cross-rpi, see Dockerfile in this dir)

mkdir -p /tmp/gbem && \
/tmp/cmake/bin/cmake -DCMAKE_TOOLCHAIN_FILE=rpi_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DGIT_TAG=$GBC_VERSION -DFLAVOUR=PI -G "CodeBlocks - Unix Makefiles" -S . -B /tmp/gbem && \
C_INCLUDE_PATH=/home/idein/cross/usr/include /tmp/cmake/bin/cmake --build /tmp/gbem --target GBEM -- -j 4 || exit 1

sudo cp /tmp/gbem/GBEM .