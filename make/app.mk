#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2014-2016, Erik Moqvist
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
INC += . $(SIMBA_ROOT)/src $(BUILDDIR)
MAIN_C ?= main.c
ifeq ($(RUST),yes)
    SRC += main.rs
else
    SRC += $(MAIN_C)
endif
SETTINGS_INI ?=
ifeq ($(SETTINGS_INI),)
SETTINGS_C ?= $(SIMBA_ROOT)/src/settings_default.c
SETTTNGS_OUTPUT_DIRECTORY ?= .
else
SETTINGS_H ?= $(GENDIR)/settings.h
SETTINGS_C ?= $(GENDIR)/settings.c
SETTTNGS_OUTPUT_DIRECTORY ?= $(GENDIR)
$(SETTTNGS_H) $(SETTINGS_C): $(SETTINGS_INI)
	$(MAKE) settings-generate
generate: $(SETTTNGS_H) $(SETTINGS_C)
endif
SRC += $(SETTINGS_C)
SETTINGS_BIN ?= $(SIMBA_ROOT)/src/settings_default.bin
SRC_FILTERED = $(filter-out $(SRC_IGNORE),$(SRC))
CSRC += $(filter %.c,$(SRC_FILTERED))
CXXSRC += $(filter %.cpp,$(SRC_FILTERED))
ASMSRC += $(filter %.S,$(SRC_FILTERED))
ASMOBJ = $(patsubst %,$(OBJDIR)%,$(abspath $(ASMSRC:%.S=%.obj)))
RUST_SRC += $(filter %.rs,$(SRC_FILTERED))
COBJ = $(patsubst %,$(OBJDIR)%,$(abspath $(CSRC:%.c=%.o)))
CXXOBJ = $(patsubst %,$(OBJDIR)%,$(abspath $(CXXSRC:%.cpp=%.o)))
RUST_OBJ = $(patsubst %,$(OBJDIR)%,$(abspath $(RUST_SRC:%.rs=%.o)))
OBJ = $(COBJ) $(CXXOBJ) $(RUST_OBJ) $(ASMOBJ)
GENCSRC = $(GENDIR)/simba_gen.c
SOAMDBAPP = $(BUILDDIR)/$(NAME).soamdb
GENOBJ = $(patsubst %,$(OBJDIR)/%,$(notdir $(GENCSRC:%.c=%.o)))
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
	index.*html \
	$(RUST_SIMBA_RS) $(RUST_LIBSIMBA) $(RUST_LIBCORE)

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
CDEFS += $(CDEFS_EXTRA)

LDFLAGS += $(LDFLAGS_EXTRA)
SHELL = /bin/bash

all:
	@echo -e "\n>>> app: $(NAME), board: $(BOARD) <<<\n"
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

CONFIG_SETTINGS_SIZE ?= 256

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
ifeq ($(SOAM), yes)
	soam.py --port $(SERIAL_PORT) --baudrate $(BAUDRATE) $(SOAMDBAPP)
else
	python -u $(CONSOLESCRIPT) --port $(SERIAL_PORT) \
				   $(CONSOLE_RESET_TYPE:%=--reset-type %) \
			           --baudrate $(BAUDRATE)
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
	env NASSERT=yes NDEBUG=yes $(MAKE)

$(EXE): $(OBJ) $(GENOBJ)
	@echo "LD $@"
	$(CXX) $(LIBPATH:%=-L%) $(LDFLAGS) -Wl,--start-group $(LIB:%=-l%) $^ -Wl,--end-group -o $@

settings-generate: $(SETTINGS_INI)
	@echo "Generating settings from $<"
	mkdir -p $(SETTTNGS_OUTPUT_DIRECTORY)
	$(SIMBA_ROOT)/bin/settings.py \
	    --output-directory $(SETTTNGS_OUTPUT_DIRECTORY) \
            --settings-size $(CONFIG_SETTINGS_SIZE) \
	    $^

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
else
	$$(CC) $$(INC:%=-I%) $$(CDEFS:%=-D%) $$(CFLAGS) -o $$@.pp.c $$< -E
endif
	$$(CC) $$(INC:%=-I%) $$(CDEFS:%=-D%) $$(CFLAGS) -o $$@ $$@.pp.c
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
else
	$$(CXX) $$(INC:%=-I%) $$(CDEFS:%=-D%) $$(CXXFLAGS) -o $$@.pp.cpp $$< -E
endif
	$$(CXX) $$(INC:%=-I%) $$(CDEFS:%=-D%) $$(CXXFLAGS) -o $$@ $$@.pp.cpp
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
	gcc -MM -MT $$@ $$(INC:%=-I%) $$(CDEFS:%=-D%) -o $(patsubst %.S,$(DEPSDIR)%.obj.dep,$(abspath $1)) $$<
endef
$(foreach file,$(ASMSRC),$(eval $(call COMPILE_ASM_template,$(file))))

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
    LIB += simba $(RUST_LIBRARIES)
    LDFLAGS += -L $(RUSTLIB)

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

$(SOAMDBAPP) $(GENOBJ): $(OBJ)
	$(SIMBA_ROOT)/bin/simbagen.py $(NAME) $(VERSION) $(BOARD_DESC) \
	$(MCU_DESC) $(GENCSRC) $(SOAMDBAPP) $(SOAMDB)
	@echo "CC $(GENCSRC)"
	$(CC) $(INC:%=-I%) $(CDEFS:%=-D%) $(CFLAGS) -o $@ $(GENCSRC)

-include local.mk
include $(SIMBA_GLOBAL_MK)

valgrind:
	valgrind --leak-check=full ./$(EXE)

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
