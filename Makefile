#
# @file Makefile
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

BOARD ?= linux

TESTS = $(addprefix kernel/,event fs prof log queue sem shell std sys thrd timer)
TESTS += $(addprefix slib/,hash_map)

all: $(TESTS:%=%.all)

release: $(TESTS:%=%.release)

clean: $(TESTS:%=%.clean)

run: $(TESTS:%=%.run)

report:
	for t in $(TESTS) ; do $(MAKE) -C tst/$(basename $$t) report ; echo ; done

test: run
	$(MAKE) report

size: $(TESTS:%=%.size)

jenkins-coverage: $(TESTS:%=%.jc)

$(TESTS:%=%.all):
	$(MAKE) -C tst/$(basename $@) all

$(TESTS:%=%.release):
	$(MAKE) -C tst/$(basename $@) release

$(TESTS:%=%.clean):
	$(MAKE) -C tst/$(basename $@) clean

$(TESTS:%=%.run):
	$(MAKE) -C tst/$(basename $@) run

$(TESTS:%=%.report):
	$(MAKE) -C tst/$(basename $@) report

$(TESTS:%=%.size):
	$(MAKE) -C tst/$(basename $@) size

$(TESTS:%=%.jc):
	$(MAKE) -C tst/$(basename $@) jenkins-coverage

help:
	@echo "--------------------------------------------------------------------------------"
	@echo "  target                      description"
	@echo "--------------------------------------------------------------------------------"
	@echo "  all                         compile and link the application"
	@echo "  clean                       remove all generated files and folders"
	@echo "  run                         run the application"
	@echo "  report                      print test report"
	@echo "  test                        run + report"
	@echo "  release                     compile with NDEBUG=yes and NPROFILE=yes"
	@echo "  size                        print executable size information"
	@echo "  help                        show this help"
	@echo
