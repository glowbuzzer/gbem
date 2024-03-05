#!/bin/sh -l

mkdir -p /tmp/gbem && \
/tmp/cmake/bin/cmake -DCMAKE_BUILD_TYPE=Release -DGIT_TAG=$GBC_VERSION -G "CodeBlocks - Unix Makefiles" -S . -B /tmp/gbem && \
C_INCLUDE_PATH=/home/idein/cross/usr/include /tmp/cmake/bin/cmake --build /tmp/gbem --target GBEM -- -j 4 || exit 1

sudo cp /tmp/gbem/GBEM .