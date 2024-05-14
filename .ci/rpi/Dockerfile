FROM idein/cross-rpi:armv6

ENV SYSROOT_CROSS ${HOME}/cross

USER root

# Update sources list to point to the Debian archive
RUN echo 'deb http://archive.debian.org/debian stretch main' > /etc/apt/sources.list && \
    echo 'deb http://archive.debian.org/debian-security stretch/updates main' >> /etc/apt/sources.list && \
    echo 'Acquire::Check-Valid-Until "false";' > /etc/apt/apt.conf.d/99no-check-valid-until && \
    apt-get update && \
    apt-get install -y libssl-dev

#RUN wget -O /tmp/libbsd-dev.deb http://mirrordirector.raspbian.org/raspbian/pool/main/libb/libbsd/libbsd-dev_0.11.7-4_armhf.deb \
# && dpkg-deb -x /tmp/libbsd-dev.deb ${SYSROOT_CROSS}/

RUN wget https://github.com/Kitware/CMake/releases/download/v3.23.1/cmake-3.23.1-linux-x86_64.sh && mkdir /tmp/cmake && sh ./cmake-3.23.1-linux-x86_64.sh --skip-license --prefix=/tmp/cmake

ENV C_INCLUDE_PATH ${SYSROOT_CROSS}/usr/include

COPY build.sh /entrypoint.sh

RUN chmod +x /entrypoint.sh
USER idein

ENTRYPOINT ["/entrypoint.sh"]