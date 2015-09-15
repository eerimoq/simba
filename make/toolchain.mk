#
# @file make/toolchain.mk
# @version 1.0
#
# @section License
# Copyright (C) 2014-2015, Erik Moqvist
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# This file is part of the Simba project.
#

ifeq ($(TOOLCHAIN), gnu)
  ifeq ($(ARCH),arm)
    CROSS_COMPILE = arm-none-eabi-
    LDFLAGS += -Wl,-spec=nosys.specs
  endif

  ifeq ($(ARCH),avr)
    CROSS_COMPILE = avr-
    CFLAGS += -D__DELAY_BACKWARD_COMPATIBLE__ -fdata-sections -ffunction-sections
    LDFLAGS += -Wl,--defsym=__main_stack_end=$(MAIN_STACK_END) -Wl,--gc-sections
    SIZEARGS = --mcu=$(MCU) --format=avr

    AVRDUDE_PORT ?= /dev/arduino
    AVRDUDE_NO_VERIFY ?= -V
    RUNARGS = ${MCU} ${NAME}.hex ${SETTINGS_BIN} -D -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) \
              $(AVRDUDE_NO_VERIFY) -b $(AVRDUDE_BAUDRATE)

    CLEAN += $(NAME).hex
    HELP_VARIABLES += "  AVRDUDE_PORT                avrdude serial port" $$(echo -e '\n') \
                      "  AVRDUDE_BAUDRATE            avrdude serial baudrate" $$(echo -e '\n')

    SIZECMD = avr-size --mcu=$(MCU) --format=avr ${EXE} ; \
              echo "Kernel package:" ; avr-size $(KERNEL_SRC:%.c=obj/%.o) -t ; echo ; \
              echo "Drivers package:" ; avr-size $(DRIVERS_SRC:%.c=obj/%.o) obj/mcu.o \
                   obj/board.o -t ; echo ; \
              echo "Slib package:" ; avr-size $(SLIB_SRC:%.c=obj/%.o) -t ; echo ; \
              echo "Other:" ; avr-size $(filter-out obj/mcu.o obj/board.o $(KERNEL_SRC:%.c=obj/%.o) \
                   $(DRIVERS_SRC:%.c=obj/%.o) $(SLIB_SRC:%.c=obj/%.o),$(OBJ) obj/simba_gen.o) -t

all: $(NAME).hex
$(NAME).hex: $(EXE)
	$(CROSS_COMPILE)objcopy -O ihex -R .eeprom $< $@

  endif

  ifeq ($(ARCH),linux)
    CROSS_COMPILE =
    CFLAGS += -Werror -Wno-error=unused-variable -DNPROFILESTACK
    LDFLAGS += -Wl,-lpthread -lrt

    ifneq ($(NPROFILE),yes)
      CFLAGS += -pg -fprofile-arcs -ftest-coverage
      LDFLAGS += -pg -fprofile-arcs -ftest-coverage -lgcov
    endif
  endif

  CC = $(CROSS_COMPILE)gcc
  LD = $(CROSS_COMPILE)gcc

  CFLAGS += -c -Wall -fstack-usage
  LDFLAGS += -Wl,-Map=$(NAME).map

  ifneq ($(NDEBUG),yes)
    CFLAGS += -g
    LDFLAGS += -g
  else
    CFLAGS += -O2
  endif
endif

CLEAN += $(NAME).map

valgrind:
	valgrind --leak-check=full ./$(EXE)
