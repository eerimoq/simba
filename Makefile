#
# @file Makefile
# @version 0.7.0
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

.PHONY: tags doc

SIMBA_VERSION ?= $(shell cat VERSION.txt)

BOARD ?= linux

# List of all tests to build and run
TESTS = $(addprefix tst/kernel/, binary_tree \
                                 bits \
                                 bus \
                                 event \
                                 fifo \
                                 fs \
                                 log \
                                 prof \
                                 queue \
                                 rwlock \
                                 sem \
                                 setting \
                                 shell \
                                 std \
                                 sys \
                                 thrd \
                                 timer)
TESTS += $(addprefix tst/slib/, base64 crc hash hash_map json re)
TESTS += $(addprefix tst/inet/, http_server \
				http_websocket_client \
				mqtt_client)

ifeq ($(BOARD), linux)
    TESTS += $(addprefix tst/kernel/, heap time)
    TESTS += $(addprefix tst/slib/, fat16)
endif

# List of all application to build
APPS = $(TESTS)

ifeq ($(ARCH),avr)
    APPS += $(addprefix tst/drivers/,adc cantp ds18b20 exti mcp2515 owi pin uart)
endif

all: $(APPS:%=%.all)

release: $(APPS:%=%.release)

clean: $(APPS:%=%.clean)

run: $(TESTS:%=%.run)

size: $(TESTS:%=%.size)

report:
	for t in $(TESTS) ; do $(MAKE) -C $(basename $$t) report ; echo ; done

test: run
	$(MAKE) report

coverage: $(TESTS:%=%.cov)
	lcov $(TESTS:%=-a %/coverage.info) -o coverage.info
	genhtml coverage.info
	@echo
	@echo "Run 'firefox index.html' to open the coverage report in a web browser."
	@echo

jenkins-coverage: $(TESTS:%=%.jc)

travis:
	$(MAKE) test

release-test:
	+bin/release.py --test
	$(MAKE) -C examples

doc:
	+bin/dbgen.py > database.json
	+bin/docgen.py database.json
	$(MAKE) -s -C doc

arduino:
	+make/arduino/arduino.py --remove-outdir --version $(SIMBA_VERSION)

$(APPS:%=%.all):
	$(MAKE) -C $(basename $@) all

$(APPS:%=%.release):
	$(MAKE) -C $(basename $@) release

$(APPS:%=%.clean):
	$(MAKE) -C $(basename $@) clean

$(APPS:%=%.size):
	$(MAKE) -C $(basename $@) size

$(TESTS:%=%.run):
	$(MAKE) -C $(basename $@) run

$(TESTS:%=%.report):
	$(MAKE) -C $(basename $@) report

$(TESTS:%=%.cov):
	$(MAKE) -C $(basename $@) coverage

$(TESTS:%=%.jc):
	$(MAKE) -C $(basename $@) jenkins-coverage

tags:
	echo "Creating tags file .TAGS"
	etags -o .TAGS --declarations $$(git ls-files *.[hci] | xargs)

cloc:
	cloc $$(git ls-files | xargs)

pmccabe:
	pmccabe $$(git ls-files "*.[hic]") | sort -n

help:
	@echo "--------------------------------------------------------------------------------"
	@echo "  target                      description"
	@echo "--------------------------------------------------------------------------------"
	@echo "  all                         compile and link the application"
	@echo "  clean                       remove all generated files and folders"
	@echo "  run                         run the application"
	@echo "  report                      print test report"
	@echo "  test                        run + report"
	@echo "  release                     compile with NASSERT=yes"
	@echo "  size                        print executable size information"
	@echo "  cloc                        print source code line statistics"
	@echo "  pmccabe                     print source code complexity statistics"
	@echo "  doc                         Generate the documentation."
	@echo "  help                        show this help"
	@echo
