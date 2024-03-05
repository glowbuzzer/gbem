FROM ubuntu:18.04

RUN apt-get update -y
RUN apt-get install -y build-essential
RUN apt-get install -y libbsd-dev
RUN apt-get install -y wget

RUN wget https://github.com/Kitware/CMake/releases/download/v3.23.1/cmake-3.23.1-linux-x86_64.sh && mkdir /tmp/cmake && sh ./cmake-3.23.1-linux-x86_64.sh --skip-license --prefix=/tmp/cmake

COPY build.sh /entrypoint.sh

RUN chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]

