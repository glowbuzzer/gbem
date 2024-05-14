FROM idein/cross-rpi:armv6

ENV SYSROOT_CROSS ${HOME}/cross

USER root

# Update sources list to point to the Debian archive
RUN echo 'deb http://archive.debian.org/debian stretch main' > /etc/apt/sources.list && \
    echo 'deb http://archive.debian.org/debian-security stretch/updates main' >> /etc/apt/sources.list && \
    echo 'Acquire::Check-Valid-Until "false";' > /etc/apt/apt.conf.d/99no-check-valid-until

# Install dependencies needed for building OpenSSL
RUN apt-get update && apt-get install -y make gcc-arm-linux-gnueabihf

# Download and unpack OpenSSL
RUN wget https://www.openssl.org/source/openssl-1.1.1k.tar.gz \
    && tar -zxf openssl-1.1.1k.tar.gz -C /tmp \
    && rm openssl-1.1.1k.tar.gz

ENV C_INCLUDE_PATH ${SYSROOT_CROSS}/usr/include

RUN cd /tmp/openssl-1.1.1k \
    && ./Configure linux-armv4 no-shared --prefix=/home/idein/cross/usr --openssldir=/home/idein/cross/usr \
        -march=armv4 -mfpu=vfp -mfloat-abi=hard \
        CC=arm-linux-gnueabihf-gcc \
    && make && make install

RUN wget https://github.com/Kitware/CMake/releases/download/v3.23.1/cmake-3.23.1-linux-x86_64.sh && mkdir /tmp/cmake && sh ./cmake-3.23.1-linux-x86_64.sh --skip-license --prefix=/tmp/cmake

COPY build.sh /entrypoint.sh

RUN chmod +x /entrypoint.sh
USER idein

RUN ls -l /usr/lib/ssl
RUN ls -l /usr/include/openssl

ENTRYPOINT ["/entrypoint.sh"]