#
# @file make/app.mk
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

.PHONY: all clean new run run-debugger help

VERSION ?= 0.0.0

# files and folders
OBJDIR = obj
DEPSDIR = deps
GENDIR = gen
INC += . $(SIMBA)/src
CSRC += $(filter %.c,$(SRC))
COBJ = $(patsubst %,$(OBJDIR)/%,$(notdir $(CSRC:%.c=%.o)))
OBJ = $(COBJ)
GENCSRC = $(GENDIR)/simba_gen.c
GENOBJ = $(patsubst %,$(OBJDIR)/%,$(notdir $(GENCSRC:%.c=%.o)))
EXE = $(NAME).out
RUNLOG = run.log
CLEAN = $(OBJDIR) $(DEPSDIR) $(GENDIR) $(EXE) $(RUNLOG) size.log \
        coverage.log coverage.xml gmon.out *.gcov profile.log \
	index.*html

# configuration
TOOLCHAIN ?= gnu
CFLAGS += $(INC:%=-I%) $(CFLAGS_EXTRA) -DARCH_$(UPPER_ARCH) -DMCU_$(UPPER_MCU) \
          -DBOARD_$(UPPER_BOARD) -DVERSION=$(VERSION)
ifeq ($(NDEBUG),yes)
  CFLAGS += -DNDEBUG
endif
ifeq ($(NPROFILE),yes)
  CFLAGS += -DNPROFILE
endif
LDFLAGS += $(LDFLAGS_EXTRA)
SHELL = /usr/bin/env bash

all: $(EXE)

# layers
BOARD.mk ?= $(SIMBA)/src/boards/$(BOARD)/board.mk
include $(BOARD.mk)
MCU.mk ?= $(SIMBA)/src/mcus/$(MCU)/mcu.mk
KERNEL.mk ?= $(SIMBA)/src/kernel/kernel.mk
DRIVERS.mk ?= $(SIMBA)/src/drivers/drivers.mk
SLIB.mk ?= $(SIMBA)/src/slib/slib.mk

include $(MCU.mk)
include $(KERNEL.mk)
include $(DRIVERS.mk)
include $(SLIB.mk)
include $(SIMBA)/make/toolchain.mk

UPPER_ARCH = $(shell echo $(ARCH) | tr a-z A-Z)
UPPER_MCU = $(shell echo $(MCU) | tr a-z A-Z)
UPPER_BOARD = $(shell echo $(BOARD) | tr a-z A-Z)

clean:
	@echo "Cleaning"
	rm -rf $(CLEAN)

new:
	$(MAKE) clean
	$(MAKE) all

run: all
	@echo "Running $(EXE)"
	set -o pipefail ; stdbuf -i0 -o0 -e0 $(SIMBA)/make/run-$(TOOLCHAIN)-$(ARCH).sh run ./$(EXE) $(SIMBA) $(RUNARGS) | tee $(RUNLOG)

dump:
	set -o pipefail ; $(SIMBA)/make/run-$(TOOLCHAIN)-$(ARCH).sh dump ./$(EXE) $(SIMBA) $(RUNARGS)

report:
	@echo "$(NAME):"
	grep "exit: test_" $(RUNLOG) | python $(SIMBA)/make/color.py || true

test: run
	$(MAKE) report

run-debugger: all
	set -o pipefail ; stdbuf -i0 -o0 -e0 $(SIMBA)/make/run-$(TOOLCHAIN)-$(ARCH).sh debugger ./$(EXE) $(SIMBA) $(RUNARGS) | tee $(RUNLOG)

profile:
	set -o pipefail ; $(SIMBA)/make/run-$(TOOLCHAIN)-$(ARCH).sh profile ./$(EXE) | tee profile.log

coverage:
	set -o pipefail ; $(SIMBA)/make/run-$(TOOLCHAIN)-$(ARCH).sh coverage ./$(EXE) | tee coverage.log

size:
	set -o pipefail ; $(SIZECMD) | tee size.log

jenkins-coverage:
	$(SIMBA)/make/run-$(TOOLCHAIN)-$(ARCH).sh jenkins-coverage ./$(EXE) > coverage.xml

release:
	env NDEBUG=yes NPROFILE=yes $(MAKE)

$(EXE): $(OBJ) $(GENOBJ)
	@echo "Linking $@"
	$(LD) -o $@ $^ $(LDFLAGS)

define COMPILE_template
-include $(patsubst %.c,$(DEPSDIR)/%.o.dep,$(notdir $1))
$(patsubst %.c,$(OBJDIR)/%.o,$(notdir $1)): $1
	@echo "Compiling $1"
	mkdir -p $(OBJDIR) $(DEPSDIR) $(GENDIR)
	$$(CC) $$(CFLAGS) -DMODULE_NAME=$(notdir $(basename $1)) -D__SIMBA_GEN__ \
	    -E -o $(patsubst %.c,$(GENDIR)/%.o.pp,$(notdir $1)) $$<
	$$(CC) $$(CFLAGS) -DMODULE_NAME=$(notdir $(basename $1)) -o $$@ $$<
	gcc -MM -MT $$@ $$(filter -I% -D% -O%,$$(CFLAGS)) -o $(patsubst %.c,$(DEPSDIR)/%.o.dep,$(notdir $1)) $$<
endef
$(foreach file,$(CSRC),$(eval $(call COMPILE_template,$(file))))

$(GENOBJ): $(OBJ)
	$(SIMBA)/src/kernel/tools/gen.py $(NAME) $(VERSION) \
	    $(GENCSRC) $(OBJ:$(OBJDIR)/%=$(GENDIR)/%.pp)
	@echo "Compiling $(GENCSRC)"
	$(CC) $(CFLAGS) -o $@ $(GENCSRC)

-include local.mk
include $(SIMBA_GLOBAL_MK)

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
