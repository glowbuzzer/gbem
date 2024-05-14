FROM idein/cross-rpi:armv6

ENV SYSROOT_CROSS ${HOME}/cross
ENV C_INCLUDE_PATH ${SYSROOT_CROSS}/usr/include

USER root

RUN wget https://www.openssl.org/source/openssl-1.1.1k.tar.gz && \
    tar -xzvf openssl-1.1.1k.tar.gz && \
    cd openssl-1.1.1k && \
    export CROSS_COMPILE=~/x-tools/armv6-rpi-linux-gnueabihf/bin/armv6-rpi-linux-gnueabihf- && \
    ./Configure linux-armv4 no-asm shared --prefix=$HOME/x-tools/armv6-rpi-linux-gnueabihf/armv6-rpi-linux-gnueabihf/sysroot --openssldir=$HOME/x-tools/armv6-rpi-linux-gnueabihf/armv6-rpi-linux-gnueabihf/sysroot/usr/openssl --cross-compile-prefix=$CROSS_COMPILE && \
    make -j4 && \
    make install

RUN wget https://github.com/Kitware/CMake/releases/download/v3.23.1/cmake-3.23.1-linux-x86_64.sh && mkdir /tmp/cmake && sh ./cmake-3.23.1-linux-x86_64.sh --skip-license --prefix=/tmp/cmake

COPY build.sh /entrypoint.sh

RUN chmod +x /entrypoint.sh
USER idein

ENTRYPOINT ["/entrypoint.sh"]
