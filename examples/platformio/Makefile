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

# The simba build system is used to run the application to check that
# it works.

TESTS = sys

SERIAL_PORT ?= /dev/arduino

test:
	for test in $(TESTS); do \
	    cd $$test ; \
	    if [ ! -e .$(BOARD).passed ] ; then \
	        platformio run -e $(BOARD) -v -t upload --upload-port $(SERIAL_PORT) || exit 1 ; \
	        $(MAKE) SERIAL_PORT=$(SERIAL_PORT) rerun || exit 1 ; \
	        touch .$(BOARD).passed ; \
	    else \
	        echo ; \
	        echo "$$test already passed." ; \
	        echo ; \
	    fi \
	done

test-prompt-after-upload:
	for test in $(TESTS); do \
	    cd $$test ; \
	    if [ ! -e .$(BOARD).passed ] ; then \
	        echo ; \
	        echo "Add any switches to force the board into the bootloader and press enter." ; \
	        echo ; \
	        read tmp ; \
	        platformio run -e $(BOARD) -v -t upload --upload-port $(SERIAL_PORT) || exit 1 ; \
	        echo ; \
	        echo "Remove any switches for forced bootloader state and press enter." ; \
	        echo ; \
	        read tmp ; \
	        $(MAKE) SERIAL_PORT=$(SERIAL_PORT) rerun || exit 1 ; \
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
	    platformio run -v -e $(BOARD) -t clean || exit 1 ; \
	    rm -f .$(BOARD).passed ; \
	done
