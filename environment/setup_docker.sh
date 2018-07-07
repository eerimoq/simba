#
# Setup script for Linux in Docker. Must be sourced from the simba
# root folder.
#

# Add the simba bin folder to the path
export PATH=$PATH:$(readlink -f bin)

# ESP8266 toolchain
export PATH=$PATH:/opt/xtensa-lx106-elf/bin

# ESP32 toolchain
export PATH=$PATH:/opt/xtensa-esp32-elf/bin

# The root of the simba repository.
export SIMBA_ROOT=$(readlink -f .)

export PYLINTRC=$(readlink -f environment/pylintrc)

# Source the user defined setup.
if [ -f ~/.simbarc ]; then
    source ~/.simbarc
fi

# ESP IDF path.
export IDF_PATH=${SIMBA_ROOT}/3pp/esp32/esp-idf

# Xvisor root.
export XVISOR_ROOT=${SIMBA_ROOT}/3pp/xvisor

# ARM toolchains.
export PATH=$PATH:/opt/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-elf/bin
export PATH=$PATH:/opt/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu/bin
