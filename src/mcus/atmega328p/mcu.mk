#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2014-2017, Erik Moqvist
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
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
