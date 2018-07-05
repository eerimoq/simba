FROM ubuntu:16.04

RUN \
    apt-get -y update && \
    apt-get install -y \
    autoconf \
    automake \
    avrdude \
    avr-libc \
    binutils-avr \
    bison \
    bossa-cli \
    bpython \
    bpython3 \
    ckermit \
    cloc \
    cppcheck \
    doxygen \
    emacs24 \
    flex \
    g++ \
    gawk \
    gcc \
    gcc-arm-none-eabi \
    gcc-avr \
    gdb-avr \
    git \
    gperf \
    help2man \
    lcov \
    libexpat-dev \
    libtool \
    libtool-bin \
    make \
    ncurses-dev \
    pmccabe \
    python \
    python3 \
    python3-pip \
    python-pip \
    python-lzma \
    python-pyelftools \
    sed \
    texinfo \
    unrar \
    unzip \
    valgrind \
    wget

RUN \
    pip install \
    breathe \
    pyserial \
    readchar \
    sphinx \
    sphinx_rtd_theme \
    xpect

RUN \
    cd /opt && \
    wget https://github.com/eerimoq/simba-releases/raw/master/arduino/esp32/tools/xtensa-esp32-elf-linux$(getconf LONG_BIT)-1.22.0-59.tar.gz && \
    tar xf xtensa-esp32-elf-linux$(getconf LONG_BIT)-1.22.0-59.tar.gz && \
    rm xtensa-esp32-elf-linux$(getconf LONG_BIT)-1.22.0-59.tar.gz

RUN mkdir tmp2

RUN groupadd -r test && useradd --no-log-init -r -g test test

RUN \
    chown -R test tmp2 && \
    chmod -R u+rX tmp2

USER test

RUN \
    cd tmp2 &&  \
    git clone --recursive https://github.com/pfalcon/esp-open-sdk && \
    cd esp-open-sdk && \
    make

USER root

RUN \
    mv /tmp2/esp-open-sdk/xtensa-lx106-elf /opt && \
    rm -rf tmp2

ENV SIMBA_DOCKER=yes