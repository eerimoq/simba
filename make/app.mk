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

.PHONY: all generate build clean new run rerun run-debugger help

VERSION ?= $(shell cat $(SIMBA_ROOT)/VERSION.txt)

all:

# files and folders
BUILDDIR ?= build/$(BOARD)
OBJDIR = $(BUILDDIR)/obj
DEPSDIR = $(BUILDDIR)/deps
GENDIR = $(BUILDDIR)/gen
INC += . $(GENDIR) $(SIMBA_ROOT)/src
MAIN_C ?= main.c
SRC += $(MAIN_C)
SETTINGS_INI ?=
SRC_FILTERED = $(filter-out $(SRC_IGNORE),$(SRC))
CSRC += $(filter %.c,$(SRC_FILTERED))
CXXSRC += $(filter %.cpp,$(SRC_FILTERED))
ASMSRC += $(filter %.S,$(SRC_FILTERED))
ASMOBJ += $(patsubst %,$(OBJDIR)%,$(abspath $(ASMSRC:%.S=%.obj)))
COBJ += $(patsubst %,$(OBJDIR)%,$(abspath $(CSRC:%.c=%.o)))
CXXOBJ += $(patsubst %,$(OBJDIR)%,$(abspath $(CXXSRC:%.cpp=%.o)))
OBJ += $(COBJ) $(CXXOBJ) $(ASMOBJ)
SIMBA_GEN_H = $(GENDIR)/simba_gen.h
SIMBA_GEN_C = $(GENDIR)/simba_gen.c
SIMBA_GEN_O = $(patsubst %,$(OBJDIR)/%,$(notdir $(SIMBA_GEN_C:%.c=%.o)))
SOAMDBAPP = $(GENDIR)/$(NAME).soamdb
ifeq ($(SOAM), yes)
SOAMDB = $(COBJ:%=%.pp.c.db) $(CXXOBJ:%=%.pp.cpp.db)
endif
EXE_SUFFIX ?= out
EXE = $(BUILDDIR)/$(NAME).$(EXE_SUFFIX)
BIN = $(BUILDDIR)/$(NAME).bin
UBIN = $(BUILDDIR)/$(NAME).ubin
UPGRADE_BINARY_DESCRIPTION ?= "$(shell date)"
HEX = $(BUILDDIR)/$(NAME).hex
MAP = $(BUILDDIR)/$(NAME).map
RUNLOG = $(BUILDDIR)/run.log
CLEAN = $(BUILDDIR) $(EXE) $(RUNLOG) size.log \
        coverage.log coverage.xml gmon.out *.gcov profile.log \
	index.*html
STUB ?=
ENDIANESS ?= little

EEPROM_SOFT_CHUNK_SIZE ?= 2048
EEPROM_BIN ?= $(GENDIR)/eeprom.bin

# configuration
TOOLCHAIN ?= gnu
CFLAGS += $(CFLAGS_EXTRA)
CXXFLAGS += $(CXXFLAGS_EXTRA)
ifeq ($(NASSERT),yes)
  CDEFS += CONFIG_ASSERT=0
endif
CDEFS += ARCH_$(UPPER_ARCH) \
	FAMILY_$(UPPER_FAMILY) \
	MCU_$(UPPER_MCU) \
	BOARD_$(UPPER_BOARD) \
	VERSION=$(VERSION) \
	MBEDTLS_USER_CONFIG_FILE="\"mbedtls/user_config.h\""
ifeq ($(SOAM),yes)
  CDEFS += SOAM
endif
ifeq ($(TYPE),suite)
  ifneq ($(NASSERT),yes)
    # Enable panic assert by default in a test suite.
    CDEFS += \
	CONFIG_PANIC_ASSERT=1
  endif
endif
CDEFS += $(CDEFS_EXTRA)

LDFLAGS += $(LDFLAGS_EXTRA)
SHELL = /bin/bash

all:
	@echo -e "\n>>> app: $(NAME), board: $(BOARD) <<<\n"
	$(MAKE) generate
	$(MAKE) build

generate: $(SIMBA_GEN_H)

build: $(EXE)

# layers
BOARD.mk ?= $(SIMBA_ROOT)/src/boards/$(BOARD)/board.mk
include $(BOARD.mk)
MCU.mk ?= $(SIMBA_ROOT)/src/mcus/$(MCU)/mcu.mk
SIMBA.mk ?= $(SIMBA_ROOT)/src/simba.mk

include $(MCU.mk)
include $(SIMBA.mk)

BAUDRATE ?= 38400

LIB ?=
LIBPATH ?=

UPPER_ARCH := $(shell python -c "import sys; sys.stdout.write(sys.argv[1].upper())" $(ARCH))
UPPER_FAMILY := $(shell python -c "import sys; sys.stdout.write(sys.argv[1].upper())" $(FAMILY))
UPPER_MCU := $(shell python -c "import sys; sys.stdout.write(sys.argv[1].upper().replace('-', '_').replace('/', '_'))" $(MCU))
UPPER_BOARD := $(shell python -c "import sys; sys.stdout.write(sys.argv[1].upper())" $(BOARD))

RUN_PY ?= $(SIMBA_ROOT)/make/run.py
RUN_END_PATTERN ?= "=============================== TEST END \(\w+\) ==============================\r\n\r\n"
RUN_END_PATTERN_SUCCESS ?= "=============================== TEST END \(PASSED\) ==============================\r\n\r\n"

CONSOLESCRIPT = $(SIMBA_ROOT)/make/console.py

clean:
	@echo "Cleaning"
	rm -rf $(CLEAN)

new:
	$(MAKE) clean
	$(MAKE) all

upload: all

# Check that the executable exists.
exe-exists:
	if [ ! -e $(EXE) ] ; then \
	    echo "The executable $(EXE) does not exist. Please build it before running it." ; \
	    exit 1 ; \
	fi

rerun: exe-exists

run: upload
	$(MAKE) rerun

console:
ifeq ($(SOAM), yes)
ifeq ($(BOARD), linux)
	soam.py tcp $(SOAMDBAPP)
else
	soam.py serial --port $(SERIAL_PORT) --baudrate $(BAUDRATE) $(SOAMDBAPP)
endif
else
	python -u $(CONSOLESCRIPT) --port $(SERIAL_PORT) \
				   $(CONSOLE_RESET_TYPE:%=--reset-type %) \
			           --baudrate $(BAUDRATE)
endif

monitor:
ifeq ($(BOARD), linux)
	socket_device.py monitor
else
	echo "Only the linux board supports this make target."
endif

report:
	@echo "$(NAME):"
	cat $(RUNLOG) | tr -cd '[:print:]\t\r\n' | egrep -e "^(exit: test|\w+_suite:)" | python $(SIMBA_ROOT)/make/color.py || true

test: run
	$(MAKE) report

size:
	set -o pipefail ; $(SIZECMD) | tee $(BUILDDIR)/size.log

size-json:
	set -o pipefail ; $(SIZE_SUMMARY_CMD) | tee $(BUILDDIR)/size-summary.log

release:
	env NASSERT=yes $(MAKE)

$(EXE): $(OBJ) $(SIMBA_GEN_O)
	@echo "LD $@"
	$(CXX) $(LIBPATH:%=-L%) $(LDFLAGS) -Wl,--start-group $(LIB:%=-l%) $^ -Wl,--end-group -o $@

define STUB_template =
$1%
endef

define COMPILE_template
-include $(patsubst %.c,$(DEPSDIR)%.o.dep,$(abspath $1))
$(patsubst %.c,$(OBJDIR)%.o,$(abspath $1)): $1
	@echo "CC $1"
	mkdir -p $(OBJDIR)$(abspath $(dir $1))
	mkdir -p $(DEPSDIR)$(abspath $(dir $1))
	mkdir -p $(GENDIR)
ifeq ($(SOAM), yes)
	$$(CC) $$(INC:%=-I%) $$(CDEFS:%=-D%) $$(CFLAGS) -DSIMBAPP -o $$@.pp $$< -E
	$(SIMBA_ROOT)/bin/simbapp.py -o $$@.pp.c $$@.pp
	$$(CC) $$(INC:%=-I%) $$(CDEFS:%=-D%) $$(CFLAGS) -o $$@ $$@.pp.c
else
	$$(CC) $$(INC:%=-I%) $$(CDEFS:%=-D%) $$(CFLAGS) -o $$@ $$<
endif
ifneq ($(STUB),)
	stub.py "$(CROSS_COMPILE)" $$@ "$$(filter $$(call STUB_template,$$<), $(STUB))"
endif
	gcc -MM -MT $$@ $$(INC:%=-I%) $$(CDEFS:%=-D%) -o $(patsubst %.c,$(DEPSDIR)%.o.dep,$(abspath $1)) $$<
endef
$(foreach file,$(CSRC),$(eval $(call COMPILE_template,$(file))))

define COMPILE_CXX_template
-include $(patsubst %.cpp,$(DEPSDIR)%.o.dep,$(abspath $1))
$(patsubst %.cpp,$(OBJDIR)%.o,$(abspath $1)): $1
	@echo "CXX $1"
	mkdir -p $(OBJDIR)$(abspath $(dir $1))
	mkdir -p $(DEPSDIR)$(abspath $(dir $1))
	mkdir -p $(GENDIR)
ifeq ($(SOAM), yes)
	$$(CXX) $$(INC:%=-I%) $$(CDEFS:%=-D%) $$(CXXFLAGS) -DSIMBAPP -o $$@.pp $$< -E
	$(SIMBA_ROOT)/bin/simbapp.py -o $$@.pp.cpp $$@.pp
	$$(CXX) $$(INC:%=-I%) $$(CDEFS:%=-D%) $$(CXXFLAGS) -o $$@ $$@.pp.cpp
else
	$$(CXX) $$(INC:%=-I%) $$(CDEFS:%=-D%) $$(CXXFLAGS) -o $$@ $$<
endif
ifneq ($(STUB),)
	stub.py "$(CROSS_COMPILE)" $$@ "$$(filter $$(call STUB_template,$$<), $(STUB))"
endif
	$$(CXX) -MM -MT $$@ $$(INC:%=-I%) $$(CDEFS:%=-D%) -std=c++11 -o $(patsubst %.cpp,$(DEPSDIR)%.o.dep,$(abspath $1)) $$<
endef
$(foreach file,$(CXXSRC),$(eval $(call COMPILE_CXX_template,$(file))))

define COMPILE_ASM_template
-include $(patsubst %.S,$(DEPSDIR)%.obj.dep,$(abspath $1))
$(patsubst %.S,$(OBJDIR)%.obj,$(abspath $1)): $1
	@echo "CC $1"
	mkdir -p $(OBJDIR)$(abspath $(dir $1))
	mkdir -p $(DEPSDIR)$(abspath $(dir $1))
	mkdir -p $(GENDIR)
	$$(CC) $$(INC:%=-I%) $$(CDEFS:%=-D%) $$(CFLAGS) -o $$@ $$<
ifneq ($(STUB),)
	stub.py "$(CROSS_COMPILE)" $$@ "$$(filter $$(call STUB_template,$$<), $(STUB))"
endif
	gcc -MM -MT $$@ $$(INC:%=-I%) $$(CDEFS:%=-D%) -o $(patsubst %.S,$(DEPSDIR)%.obj.dep,$(abspath $1)) $$<
endef
$(foreach file,$(ASMSRC),$(eval $(call COMPILE_ASM_template,$(file))))

$(SIMBA_GEN_H): $(SETTINGS_INI)
	@echo "GEN $@"
	mkdir -p $(GENDIR)
	$(SIMBA_ROOT)/bin/simbagen.py \
	    --output-directory $(GENDIR) \
	    header \
	    --endianess $(ENDIANESS) \
	    $(<:%=--settings %)

$(SOAMDBAPP) $(SIMBA_GEN_O): $(OBJ)
	@echo "GEN $(SIMBA_GEN_C)"
	$(SIMBA_ROOT)/bin/simbagen.py \
	    --output-directory $(GENDIR) \
	    source \
	    --name $(NAME) \
	    --version $(VERSION) \
	    --board $(BOARD_DESC) \
	    --mcu $(MCU_DESC) \
	    --endianess $(ENDIANESS) \
	    --eeprom-soft-chunk-size $(EEPROM_SOFT_CHUNK_SIZE) \
	    $(SETTINGS_INI:%=--settings %) \
	    $(SOAMDB)
	@echo "CC $(SIMBA_GEN_C)"
	$(CC) $(INC:%=-I%) $(CDEFS:%=-D%) $(CFLAGS) -o $@ $(SIMBA_GEN_C)

-include local.mk
include $(SIMBA_GLOBAL_MK)

valgrind:
	valgrind --leak-check=full ./$(EXE)

stack-usage:
	cat $$(find $(BUILDDIR)/obj -name "*.su") | sort -n -k2

CPPCHECK_ENABLE = warning performance portability information

cppcheck:
	(for path in $(CSRC); do echo $$path; done) | \
	cppcheck --std=c99 $(CPPCHECK_ENABLE:%=--enable=%) $(INC:%=-I%) \
	$(CDEFS:%=-D%) --file-list=- --template=gcc --error-exitcode=1

help:
	@echo "--------------------------------------------------------------------------------"
	@echo "  target                      description"
	@echo "--------------------------------------------------------------------------------"
	@echo "  all                         Compile and link the application."
	@echo "  clean                       Remove all generated files and folders."
	@echo "  new                         clean + all"
	@echo "  upload                      all + Upload the application to the device."
	@echo "  run                         upload + Run the application."
	@echo "  run-debugger                Run the application in the debugger, break at main."
	@echo "  rerun                       Run the application wihtout uploading it."
	@echo "  report                      Print the test report."
	@echo "  test                        run + report"
	@echo "  console                     Open a serial console on /dev/arduino with"
	@echo "                              baudrate BAUDRATE."
	@echo "  release                     Compile with NASSERT=yes."
	@echo "  size                        Print application size information."
	@echo "  stack-usage                 Print stack usage per function."
	@echo "  backtrace                   Convert a list of space separated addresses in "
	@echo "                              BACKTRACE to a human readable string."

	@IFS=$$'\n' ; for h in $(HELP_TARGETS) ; do \
	  echo $$h ; \
	done
	@echo "  help                        Show this help."
	@echo "--------------------------------------------------------------------------------"
	@echo "  variable                    description"
	@echo "--------------------------------------------------------------------------------"
	@echo "  NASSERT                      yes - build without assertions"
	@IFS=$$'\n' ; for h in $(HELP_VARIABLES) ; do \
	  echo $$h ; \
	done
	@echo

print-%:
	@echo $($*)

default-configuration:
	gcc -E -dM $(CDEFS:%=-D%) $(SIMBA_ROOT)/src/config_default.h \
		| grep "#define CONFIG"_ \
		| grep -v "#define __CONFIG" \
		| grep -v -P "#define CONFIG_START_CONSOLE_NONE |#define CONFIG_START_CONSOLE_UART |#define CONFIG_START_CONSOLE_USB_CDC "

port-has:
	gcc -E -dM $(CDEFS:%=-D%) $(SIMBA_ROOT)/src/config_default.h \
		| grep "#define PORT_HAS_"
