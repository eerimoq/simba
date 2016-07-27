#
# Setup script for Linux. Must be sourced from the simba root folder.
#

# Add the simba bin folder to the path
export PATH=$PATH:$(readlink -f bin)

# ESP8266 toolchain
export PATH=$PATH:$(readlink -f ../esp-open-sdk/xtensa-lx106-elf/bin)

# RUST bindgen
export PATH=$PATH:$(readlink -f ../rust-bindgen/target/debug)

# st-link (STM32)
export PATH=$PATH:$(readlink -f ../stlink)

# dfu-util (Photon)
export PATH=$PATH:$(readlink -f ../dfu-util/src)

export SIMBA_ROOT=$(readlink -f .)

export PYLINTRC=$(readlink -f environment/pylintrc)

# Arduino release repository.
export SIMBA_ARDUINO_ROOT=$(readlink -f ../simba-arduino)

# Arduino IDE folder.
export ARDUINO_ROOT=$(readlink -f ../../arduino-1.6.10)
