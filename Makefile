#
# @file Makefile
# @version 1.0.0
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

ifeq ($(BOARD), linux)
    TESTS = $(addprefix tst/kernel/, binary_tree \
                                     bits \
                                     bus \
                                     event \
                                     fifo \
                                     fs \
				     heap \
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
    TESTS += $(addprefix tst/slib/, base64 \
				    crc \
				    fat16 \
				    hash \
				    hash_map \
				    json \
				    re)
    TESTS += $(addprefix tst/inet/, http_server \
				    http_websocket_client \
				    inet \
				    mqtt_client)
endif

ifeq ($(BOARD), arduino_due)
    TESTS = $(addprefix tst/kernel/, binary_tree \
                                     bits \
                                     bus \
                                     event \
                                     fifo \
                                     fs \
				     heap \
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
    TESTS += $(addprefix tst/slib/, base64 \
				    crc \
				    hash \
				    hash_map \
				    json \
				    re)
    TESTS += $(addprefix tst/inet/, http_server \
				    http_websocket_client \
				    inet \
				    mqtt_client)
    TESTS += $(addprefix tst/drivers/, chipid \
				       can \
				       flash)
endif

ifeq ($(BOARD), arduino_mega)
    TESTS = $(addprefix tst/kernel/, binary_tree \
                                     bits \
                                     bus \
                                     event \
                                     fifo \
                                     fs \
				     heap \
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
    TESTS += $(addprefix tst/slib/, base64 \
				    crc \
				    hash \
				    hash_map \
				    json \
				    re)
    TESTS += $(addprefix tst/inet/, http_server \
				    http_websocket_client \
				    inet \
				    mqtt_client)
    TESTS += $(addprefix tst/drivers/, adc \
				       ds3231)
endif

ifeq ($(BOARD), arduino_nano)
    TESTS = $(addprefix tst/drivers/, exti \
				      pin)
    APPS = $(addprefix tst/drivers/, ds18b20 \
				     owi \
				     uart)
endif

ifeq ($(BOARD), stm32vldiscovery)
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
                                     shell \
                                     std \
                                     sys \
                                     thrd \
                                     timer)
    TESTS += $(addprefix tst/slib/, base64 \
				    crc \
				    hash \
				    hash_map \
				    re)
    TESTS += $(addprefix tst/inet/, http_websocket_client \
				    inet \
				    mqtt_client)
endif

# List of all application to build
APPS += $(TESTS)

all: $(APPS:%=%.all)

release: $(APPS:%=%.release)

clean: $(APPS:%=%.clean)

run: all
	for test in $(TESTS) ; do \
	    $(MAKE) -C $$test run || exit 1 ; \
	done

size: $(TESTS:%=%.size)

report:
	for test in $(TESTS) ; do \
	    $(MAKE) -C $(basename $$test) report ; \
	    echo ; \
	done

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
	+bin/release.py

clean-arduino-due:
	$(MAKE) BOARD=arduino_due SERIAL_PORT=simba-arduino_due clean

clean-arduino-mega:
	$(MAKE) BOARD=arduino_mega SERIAL_PORT=simba-arduino_mega clean

clean-arduino-nano:
	$(MAKE) BOARD=arduino_nano SERIAL_PORT=simba-arduino_nano clean

clean-stm32vldiscovery:
	$(MAKE) BOARD=stm32vldiscovery SERIAL_PORT=simba-stm32vldiscovery clean

clean-esp12e:
	$(MAKE) BOARD=esp12e SERIAL_PORT=simba-esp12e clean

test-arduino-due:
	@echo "Arduino Due"
	$(MAKE) BOARD=arduino_due SERIAL_PORT=simba-arduino_due test

test-arduino-mega:
	@echo "Arduino Mega"
	$(MAKE) BOARD=arduino_mega SERIAL_PORT=simba-arduino_mega test

test-arduino-nano:
	@echo "Arduino Nano"
	$(MAKE) BOARD=arduino_nano SERIAL_PORT=simba-arduino_nano test

test-stm32vldiscovery:
	@echo "STM32VLDISCOVERY"
	$(MAKE) BOARD=stm32vldiscovery SERIAL_PORT=simba-stm32vldiscovery test

test-esp12e:
	@echo "ESP12-E"
	$(MAKE) BOARD=esp12e SERIAL_PORT=simba-esp12e test

test-all-boards:
	$(MAKE) clean-arduino-due
	$(MAKE) test-arduino-due
	$(MAKE) clean-arduino-mega
	$(MAKE) test-arduino-mega
	$(MAKE) clean-arduino-nano
	$(MAKE) test-arduino-nano
	$(MAKE) clean-stm32vldiscovery
	$(MAKE) test-stm32vldiscovery
#	$(MAKE) test-all-boards: test-esp12e

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
