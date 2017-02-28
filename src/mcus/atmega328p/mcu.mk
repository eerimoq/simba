#
# @section License
#
# This is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.
#
# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# For more information, please refer to <http://unlicense.org/>
#
# This file is part of the Simba project.
#

INC += $(SIMBA_ROOT)/src/mcus/atmega328p
SRC += $(SIMBA_ROOT)/src/mcus/atmega328p/mcu.c

AVRDUDE_BAUDRATE = 57600
AVRDUDE_PROGRAMMER = arduino

F_CPU = 16000000
CPU = atmega328p
MAIN_STACK_END = 0x800900

ARCH = avr
FAMILY = avr

MCU_HOMEPAGE = "http://www.atmel.com"
MCU_NAME = "Atmel ATMega328p"
MCU_DESC = "Atmel ATMega328p AVR @ 16MHz, 2k sram, 32k flash"

include $(SIMBA_ROOT)/make/$(TOOLCHAIN)/avr.mk
