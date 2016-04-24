#
# @file make/app.mk
# @version 0.3.0
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

.PHONY: all generate build clean new run run-debugger help

VERSION ?= $(shell cat $(SIMBA_ROOT)/VERSION.txt)

# files and folders
OBJDIR = obj
DEPSDIR = deps
GENDIR = gen
INC += . $(SIMBA_ROOT)/src
ifeq ($(RUST),yes)
    SRC += main.rs
else
    SRC += main.c
endif
LIBPATH ?=
SRC_FILTERED = $(filter-out $(SRC_IGNORE),$(SRC))
CSRC += $(filter %.c,$(SRC_FILTERED))
RUST_SRC += $(filter %.rs,$(SRC_FILTERED))
COBJ = $(patsubst %,$(OBJDIR)%,$(abspath $(CSRC:%.c=%.o)))
RUST_OBJ = $(patsubst %,$(OBJDIR)%,$(abspath $(RUST_SRC:%.rs=%.o)))
OBJ = $(COBJ) $(RUST_OBJ)
GENCSRC = $(GENDIR)/simba_gen.c
GENOBJ = $(patsubst %,$(OBJDIR)/%,$(notdir $(GENCSRC:%.c=%.o)))
SETTINGS_INI ?= $(SIMBA_ROOT)/make/settings.ini
SETTINGS_H = settings.h
SETTINGS_C = settings.c
SETTINGS_BIN = settings.bin
EXE = $(NAME).out
RUNLOG = run.log
CLEAN = $(OBJDIR) $(DEPSDIR) $(GENDIR) $(EXE) $(RUNLOG) size.log \
        coverage.log coverage.xml gmon.out *.gcov profile.log \
	index.*html $(SETTINGS_H) $(SETTINGS_BIN) $(SETTINGS_C) \
	$(RUST_SIMBA_RS) $(RUST_LIBSIMBA) $(RUST_LIBCORE)

# configuration
TOOLCHAIN ?= gnu
CFLAGS += $(INC:%=-I%) $(CFLAGS_EXTRA)
ifeq ($(NDEBUG),yes)
  CDEFS += -DNDEBUG
endif
ifeq ($(NPROFILE),yes)
  CDEFS += -DNPROFILE
endif
CDEFS +=  -DARCH_$(UPPER_ARCH) -DMCU_$(UPPER_MCU) \
          -DBOARD_$(UPPER_BOARD) -DVERSION=$(VERSION)
CFLAGS += $(CDEFS)
LDFLAGS += $(LIBPATH:%=-L%) $(LDFLAGS_EXTRA)
SHELL = /bin/bash

all:
	$(MAKE) prepare
	$(MAKE) generate
	$(MAKE) build
	$(MAKE) postpare

prepare:

generate: $(SETTINGS_BIN) $(SETTINGS_H)

build: $(EXE)

postpare:

# layers
BOARD.mk ?= $(SIMBA_ROOT)/src/boards/$(BOARD)/board.mk
include $(BOARD.mk)
MCU.mk ?= $(SIMBA_ROOT)/src/mcus/$(MCU)/mcu.mk
KERNEL.mk ?= $(SIMBA_ROOT)/src/kernel/kernel.mk
DRIVERS.mk ?= $(SIMBA_ROOT)/src/drivers/drivers.mk
SLIB.mk ?= $(SIMBA_ROOT)/src/slib/slib.mk

include $(MCU.mk)
include $(KERNEL.mk)
include $(DRIVERS.mk)
include $(SLIB.mk)

UPPER_ARCH := $(shell python -c "import sys; sys.stdout.write(sys.argv[1].upper())" $(ARCH))
UPPER_MCU := $(shell python -c "import sys; sys.stdout.write(sys.argv[1].upper().replace('-', '_').replace('/', '_'))" $(MCU))
UPPER_BOARD := $(shell python -c "import sys; sys.stdout.write(sys.argv[1].upper())" $(BOARD))

RUNSCRIPT = $(SIMBA_ROOT)/make/$(TOOLCHAIN)/$(ARCH).py
RUN_END_PATTERN ?= "harness report: total\(\d+\), passed\(\d+\), failed\(\d+\)"
RUN_END_PATTERN_SUCCESS ?= "harness report: total\(\d+\), passed\(\d+\), failed\(0\)"

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

run: all
	@echo "Running $(EXE)"
	python -u $(RUNSCRIPT) run ./$(EXE) $(SIMBA_ROOT) \
                  $(RUNLOG) $(RUN_END_PATTERN) $(RUN_END_PATTERN_SUCCESS) \
                  $(RUNARGS)

upload: all
	@echo "Uploading $(EXE)"
	python -u $(RUNSCRIPT) upload ./$(EXE) $(SIMBA_ROOT) \
                  $(RUNLOG) $(RUN_END_PATTERN) $(RUN_END_PATTERN_SUCCESS) \
                  $(RUNARGS)

dump:
	python -u $(RUNSCRIPT) dump ./$(EXE) $(SIMBA_ROOT) $(RUNARGS)

report:
	@echo "$(NAME):"
	grep "exit: test_" $(RUNLOG) | python $(SIMBA_ROOT)/make/color.py || true

test: run
	$(MAKE) report

run-debugger: all
	python -u $(RUNSCRIPT) debugger ./$(EXE) $(SIMBA_ROOT) $(RUNLOG) $(RUNARGS)

profile:
	python -u $(RUNSCRIPT) profile ./$(EXE) $(SIMBA_ROOT) profile.log

coverage:
	python -u $(RUNSCRIPT) coverage ./$(EXE) $(SIMBA_ROOT) coverage.log

size:
	set -o pipefail ; $(SIZECMD) | tee size.log

jenkins-coverage:
	$(RUNSCRIPT) jenkins-coverage ./$(EXE) > coverage.xml

release:
	env NDEBUG=yes NPROFILE=yes $(MAKE)

$(EXE): $(OBJ) $(GENOBJ)
	@echo "Linking $@"
	$(LD) -o $@ $(LDFLAGS) $^ $(LDFLAGS_AFTER)

$(SETTINGS_H): $(SETTINGS_INI)
	@echo "Generating $@ from $<"
	$(SIMBA_ROOT)/src/kernel/tools/settings.py --header $(SETTINGS_INI) \
            --setting-memory $(SETTING_MEMORY) --setting-offset $(SETTING_OFFSET) \
            --setting-size $(SETTING_SIZE) $(ENDIANESS)

$(SETTINGS_C): $(SETTINGS_INI)
	@echo "Generating $@ from $<"
	$(SIMBA_ROOT)/src/kernel/tools/settings.py --source $(SETTINGS_INI) \
            --setting-memory $(SETTING_MEMORY) --setting-offset $(SETTING_OFFSET) \
            --setting-size $(SETTING_SIZE) $(ENDIANESS)

$(SETTINGS_BIN): $(SETTINGS_INI)
	@echo "Generating $@ from $<"
	$(SIMBA_ROOT)/src/kernel/tools/settings.py --binary $(SETTINGS_INI) \
            --setting-memory $(SETTING_MEMORY) --setting-offset $(SETTING_OFFSET) \
            --setting-size $(SETTING_SIZE) $(ENDIANESS)

define COMPILE_template
-include $(patsubst %.c,$(DEPSDIR)%.o.dep,$(abspath $1))
$(patsubst %.c,$(OBJDIR)%.o,$(abspath $1)): $1 $(SETTINGS_H)
	@echo "Compiling $1"
	mkdir -p $(OBJDIR)$(abspath $(dir $1))
	mkdir -p $(DEPSDIR)$(abspath $(dir $1))
	mkdir -p $(GENDIR)$(abspath $(dir $1))
	$$(CC) $$(CFLAGS) -DMODULE_NAME=$(notdir $(basename $1)) -D__SIMBA_GEN__ \
	    -E -o $(patsubst %.c,$(GENDIR)%.o.pp,$(abspath $1)) $$<
	$$(CC) $$(CFLAGS) -DMODULE_NAME=$(notdir $(basename $1)) -o $$@ $$<
	gcc -MM -MT $$@ $$(filter -I% -D% -O%,$$(CFLAGS)) -o $(patsubst %.c,$(DEPSDIR)%.o.dep,$(abspath $1)) $$<
endef
$(foreach file,$(CSRC),$(eval $(call COMPILE_template,$(file))))

RUSTLIB = rustlib
RUST_ROOT ?= $(SIMBA_ROOT)/rust/rust
RUST_TARGET ?=
RUST_SIMBA_RS = $(SIMBA_ROOT)/rust/src/simba.rs
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
ifeq ($(RUST_TARGET),)
	cp /usr/local/lib/rustlib/*-unknown-linux-gnu/lib/libcore-*.rlib $@
else
	rustc $(RSFLAGS) $(RUST_ROOT)/src/$(@:$(RUSTLIB)/%.a=%)/lib.rs --out-dir .
	mkdir -p $(RUSTLIB)
	cp $(@:$(RUSTLIB)/%.a=%.rlib) $@
endif

$(RUSTLIB)/$(RUST_LIBSIMBA): $(RUST_SIMBA_RS) $(RUST_LIBSIMBA_SRC:%=$(SIMBA_ROOT)/rust/src/%)
	@echo "Compiling RUST $@..."
	rustc $(RSFLAGS) $(SIMBA_ROOT)/rust/src/lib.rs --crate-name simba --out-dir .
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
	    $(GENCSRC) $(OBJ:$(OBJDIR)/%=$(GENDIR)/%.pp)
	@echo "Compiling $(GENCSRC)"
	$(CC) $(CFLAGS) -o $@ $(GENCSRC)

-include local.mk
include $(SIMBA_GLOBAL_MK)

valgrind:
	valgrind --leak-check=full ./$(EXE)

CPPCHECK_ENABLE = warning performance portability information

cppcheck:
	(for path in $(CSRC); do echo $$path; done) | \
	cppcheck --std=c99 $(CPPCHECK_ENABLE:%=--enable=%) $(INC:%=-I%) $(CDEFS) \
	--file-list=- --template=gcc --error-exitcode=1

help:
	@echo "--------------------------------------------------------------------------------"
	@echo "  target                      description"
	@echo "--------------------------------------------------------------------------------"
	@echo "  all                         compile and link the application"
	@echo "  clean                       remove all generated files and folders"
	@echo "  new                         clean + all"
	@echo "  run                         run the application"
	@echo "  run-debugger                run the application in the debugger, break at main"
	@echo "  report                      print test report"
	@echo "  test                        run + report"
	@echo "  release                     compile with NDEBUG=yes and NPROFILE=yes"
	@echo "  size                        print executable size information"
	@IFS=$$'\n' ; for h in $(HELP_TARGETS) ; do \
	  echo $$h ; \
	done
	@echo "  help                        show this help"
	@echo "--------------------------------------------------------------------------------"
	@echo "  variable                    description"
	@echo "--------------------------------------------------------------------------------"
	@echo "  NDEBUG                      yes - build without debug information"
	@echo "  NPROFILE                    yes - build without profiling information"
	@IFS=$$'\n' ; for h in $(HELP_VARIABLES) ; do \
	  echo $$h ; \
	done
	@echo

print-%:
	@echo $($*)
