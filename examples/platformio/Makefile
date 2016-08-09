#
# @file Makefile
# @version 4.1.0
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

# The simba build system is used to run the application to check that
# it works.

TESTS = sys

SERIAL_PORT ?= /dev/arduino

upload-test:
	for test in $(TESTS); do \
	    cd $$test ; \
	    if [ ! -e .$(BOARD).passed ] ; then \
	        platformio run -e $(BOARD) -t upload --upload-port $(SERIAL_PORT) || exit 1 ; \
	        $(MAKE) run SERIAL_PORT=$(SERIAL_PORT) || exit 1 ; \
	        touch .$(BOARD).passed ; \
	    else \
	        echo ; \
	        echo "$$test already passed." ; \
	        echo ; \
	    fi \
	done

upload-test-prompt-after-upload:
	for test in $(TESTS); do \
	    cd $$test ; \
	    if [ ! -e .$(BOARD).passed ] ; then \
	        echo ; \
	        echo "Add any switches to force the board into the bootloader and press enter." ; \
	        echo ; \
	        read tmp ; \
	        platformio run -e $(BOARD) -t upload --upload-port $(SERIAL_PORT) || exit 1 ; \
	        echo ; \
	        echo "Remove any switches for forced bootloader state and press enter." ; \
	        echo ; \
	        read tmp ; \
	        $(MAKE) run SERIAL_PORT=$(SERIAL_PORT) || exit 1 ; \
	        touch .$(BOARD).passed ; \
	    else \
	        echo ; \
	        echo "$$test already passed." ; \
	        echo ; \
	    fi \
	done

clean:
	for test in $(TESTS); do \
	    cd $$test ; \
	    platformio run -e $(BOARD) -t clean || exit 1 ; \
	    rm -f .$(BOARD).passed ; \
	done
