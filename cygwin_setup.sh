#
# Setup script for Cygwin on Windows.
#

# AVR toolchain
export PATH=$PATH:$(readlink -f ../avr-toolchain-windows/bin)

# ARM toolchain
export PATH=$PATH:$(readlink -f ../arm-toolchain-windows/arm-none-eabi-gcc/4.8.3-2014q1/bin)
