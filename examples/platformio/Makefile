#
# @section License
#
# This is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.
#
# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# For more information, please refer to <http://unlicense.org/>
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
