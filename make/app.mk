#
# @file make/app.mk
# @version 6.0.0
#
# @section License
# Copyright (C) 2014-2016, Erik Moqvist
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

.PHONY: all generate build clean new run rerun run-debugger help

VERSION ?= $(shell cat $(SIMBA_ROOT)/VERSION.txt)

CONFIG_SETTING_SIZE ?= 256

# files and folders
BUILDDIR ?= build/$(BOARD)
OBJDIR = $(BUILDDIR)/obj
DEPSDIR = $(BUILDDIR)/deps
GENDIR = $(BUILDDIR)/gen
INC += . $(SIMBA_ROOT)/src $(BUILDDIR)
ifeq ($(RUST),yes)
    SRC += main.rs
else
    SRC += main.c
endif
SRC += $(SIMBA_ROOT)/src/settings.c
SETTINGS_BIN ?= $(SIMBA_ROOT)/src/settings.bin
SETTTNGS_OUTPUT_DIRECTORY ?= .
SRC_FILTERED = $(filter-out $(SRC_IGNORE),$(SRC))
CSRC += $(filter %.c,$(SRC_FILTERED))
RUST_SRC += $(filter %.rs,$(SRC_FILTERED))
COBJ = $(patsubst %,$(OBJDIR)%,$(abspath $(CSRC:%.c=%.o)))
RUST_OBJ = $(patsubst %,$(OBJDIR)%,$(abspath $(RUST_SRC:%.rs=%.o)))
OBJ = $(COBJ) $(RUST_OBJ)
GENCSRC = $(GENDIR)/simba_gen.c
GENOBJ = $(patsubst %,$(OBJDIR)/%,$(notdir $(GENCSRC:%.c=%.o)))
SETTINGS_INI ?= $(SIMBA_ROOT)/make/settings.ini
EXE_SUFFIX ?= out
EXE = $(BUILDDIR)/$(NAME).$(EXE_SUFFIX)
BIN = $(BUILDDIR)/$(NAME).bin
HEX = $(BUILDDIR)/$(NAME).hex
MAP = $(BUILDDIR)/$(NAME).map
RUNLOG = $(BUILDDIR)/run.log
CLEAN = $(BUILDDIR) $(EXE) $(RUNLOG) size.log \
        coverage.log coverage.xml gmon.out *.gcov profile.log \
	index.*html \
	$(RUST_SIMBA_RS) $(RUST_LIBSIMBA) $(RUST_LIBCORE)

# configuration
TOOLCHAIN ?= gnu
CFLAGS += $(CFLAGS_EXTRA)
ifeq ($(NASSERT),yes)
  CDEFS += CONFIG_ASSERT=0
endif
CDEFS += ARCH_$(UPPER_ARCH) \
	FAMILY_$(UPPER_FAMILY) \
	MCU_$(UPPER_MCU) \
	BOARD_$(UPPER_BOARD) \
	VERSION=$(VERSION)
CDEFS += $(CDEFS_EXTRA)

LDFLAGS += $(LDFLAGS_EXTRA)
SHELL = /bin/bash

BAUDRATE ?= 38400

all:
	@echo -e "\n>>> $(NAME) <<<\n"
	$(MAKE) prepare
	$(MAKE) generate
	$(MAKE) build
	$(MAKE) postpare

prepare:

generate:

build: $(EXE)

postpare:

# layers
BOARD.mk ?= $(SIMBA_ROOT)/src/boards/$(BOARD)/board.mk
include $(BOARD.mk)
MCU.mk ?= $(SIMBA_ROOT)/src/mcus/$(MCU)/mcu.mk
SIMBA.mk ?= $(SIMBA_ROOT)/src/simba.mk

include $(MCU.mk)
include $(SIMBA.mk)

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

# include packages in dist-packages used by the application
define DIST_PACKAGES_template
$(shell python -c "import sys; sys.stdout.write(sys.argv[1].upper() + '_ROOT')" $1) ?= \
    $(SIMBA_ROOT)/dist-packages/$1
include $(SIMBA_ROOT)/dist-packages/$1/src/$1.mk
endef
$(foreach name,$(DIST_PACKAGES),$(eval $(call DIST_PACKAGES_template,$(name))))

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
	python -u $(CONSOLESCRIPT) --port $(SERIAL_PORT) \
				   $(CONSOLE_RESET_TYPE:%=--reset-type %) \
			           --baudrate $(BAUDRATE)

report:
	@echo "$(NAME):"
	cat $(RUNLOG) | tr -cd '[:print:]\t\r\n' | grep "exit: test_" | python $(SIMBA_ROOT)/make/color.py || true

test: run
	$(MAKE) report

size:
	set -o pipefail ; $(SIZECMD) | tee $(BUILDDIR)/size.log

release:
	env NASSERT=yes NDEBUG=yes $(MAKE)

$(EXE): $(OBJ) $(GENOBJ)
	@echo "Linking $@"
	$(CC) -o $@ $(LIBPATH:%=-L%) $(LDFLAGS) $^ $(LDFLAGS_AFTER)

settings-generate: $(SETTINGS_INI)
	@echo "Generating settings from $<"
	mkdir -p $(SETTTNGS_OUTPUT_DIRECTORY)
	$(SIMBA_ROOT)/bin/settings.py \
	    --output-directory $(SETTTNGS_OUTPUT_DIRECTORY) \
            --setting-size $(CONFIG_SETTING_SIZE) \
	    $^

define COMPILE_template
-include $(patsubst %.c,$(DEPSDIR)%.o.dep,$(abspath $1))
$(patsubst %.c,$(OBJDIR)%.o,$(abspath $1)): $1
	@echo "Compiling $1"
	mkdir -p $(OBJDIR)$(abspath $(dir $1))
	mkdir -p $(DEPSDIR)$(abspath $(dir $1))
	mkdir -p $(GENDIR)
	$$(CC) $$(INC:%=-I%) $$(CDEFS:%=-D%) $$(CFLAGS) -o $$@ $$<
	gcc -MM -MT $$@ $$(INC:%=-I%) $$(CDEFS:%=-D%) -o $(patsubst %.c,$(DEPSDIR)%.o.dep,$(abspath $1)) $$<
endef
$(foreach file,$(CSRC),$(eval $(call COMPILE_template,$(file))))

RUSTLIB = rustlib
RUST_ROOT ?= $(SIMBA_ROOT)/rost/rust
RUST_TARGET ?=
RUST_SIMBA_RS = $(SIMBA_ROOT)/rost/src/simba.rs
RUST_LIBSIMBA = libsimba.a
RUST_LIBSIMBA_SRC = \
	lib.rs \
	kernel/chan.rs \
	kernel/event.rs \
	kernel/mod.rs \
	kernel/queue.rs \
	kernel/sys.rs \
	kernel/thrd.rs \
	drivers/mod.rs \
	drivers/uart.rs \
	slib/mod.rs \
	slib/harness.rs
RUST_LIBRARIES = \
	core \
	alloc \
	rustc_unicode \
	collections
RSFLAGS = \
	-g \
	-Z no-landing-pads \
	-C opt-level=3 \
	-L . \
	--crate-type lib \
	$(RUST_TARGET:%=--target=$(SIMBA_ROOT)/make/rust/%.json) \
	$(RSFLAGS_EXTRA)

ifeq ($(RUST), yes)
    LDFLAGS_AFTER += -lsimba $(RUST_LIBRARIES:%=-l%) -L $(RUSTLIB)

generate: $(RUST_LIBRARIES:%=$(RUSTLIB)/lib%.a) $(RUSTLIB)/$(RUST_LIBSIMBA)
endif

$(RUST_LIBRARIES:%=$(RUSTLIB)/lib%.a):
	@echo "Compiling RUST library $@..."
	mkdir -p $(RUSTLIB)
ifeq ($(RUST_TARGET),)
	cp /usr/local/lib/rustlib/*-unknown-linux-gnu/lib/$(@:$(RUSTLIB)/%.a=%)-*.rlib $@
else
	rustc $(RSFLAGS) $(RUST_ROOT)/src/$(@:$(RUSTLIB)/%.a=%)/lib.rs --out-dir .
	cp $(@:$(RUSTLIB)/%.a=%.rlib) $@
endif

$(RUSTLIB)/$(RUST_LIBSIMBA): $(RUST_SIMBA_RS) $(RUST_LIBSIMBA_SRC:%=$(SIMBA_ROOT)/rost/src/%)
	@echo "Compiling RUST $@..."
	rustc $(RSFLAGS) $(SIMBA_ROOT)/rost/src/lib.rs --crate-name simba --out-dir .
	cp libsimba.rlib $@

-include $(DEPSDIR)/simba.rs.dep
$(RUST_SIMBA_RS):
	@echo "Generating RUST $@"
	mkdir -p $(DEPSDIR)
	bindgen -builtins -core $(filter -I% -D% -O%,$(CFLAGS)) -o $@ $(SIMBA_ROOT)/src/simba.h
	gcc -MM -MT $@ $(filter -I% -D% -O%,$(CFLAGS)) -o $(DEPSDIR)/simba.rs.dep $(SIMBA_ROOT)/src/simba.h

define RUST_COMPILE_template
-include $(patsubst %.rs,$(DEPSDIR)%.o.dep,$(abspath $1))
$(patsubst %.rs,$(OBJDIR)%.o,$(abspath $1)): $1 $(SIMBA_RS)
	@echo "Compiling $1"
	mkdir -p $(OBJDIR)$(abspath $(dir $1))
	mkdir -p $(DEPSDIR)$(abspath $(dir $1))
	mkdir -p $(GENDIR)$(abspath $(dir $1))
	cp $$< $(patsubst %.rs,$(GENDIR)%.o.pp,$(abspath $1))
	rustc $(RSFLAGS) -o $$@ $$<
endef
$(foreach file,$(RUST_SRC),$(eval $(call RUST_COMPILE_template,$(file))))

$(GENOBJ): $(OBJ)
	$(SIMBA_ROOT)/src/kernel/tools/gen.py $(NAME) $(VERSION) $(BOARD_DESC) $(MCU_DESC) \
	    $(GENCSRC)
	@echo "Compiling $(GENCSRC)"
	$(CC) $(INC:%=-I%) $(CDEFS:%=-D%) $(CFLAGS) -o $@ $(GENCSRC)

-include local.mk
include $(SIMBA_GLOBAL_MK)

valgrind:
	valgrind --leak-check=full ./$(EXE)

CPPCHECK_ENABLE = warning performance portability information

cppcheck:
	(for path in $(CSRC); do echo $$path; done) | \
	cppcheck --std=c99 $(CPPCHECK_ENABLE:%=--enable=%) $(INC:%=-I%) $(CDEFS:%=-D%) \
	--file-list=- --template=gcc --error-exitcode=1

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
	@echo "  release                     Compile with NASSERT=yes and NDEBUG=yes."
	@echo "  size                        Print application size information."
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
