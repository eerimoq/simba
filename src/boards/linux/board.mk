#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2014-2017, Erik Moqvist
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

.PHONY: coverage

PINOUT ?= linux

INC += $(SIMBA_ROOT)/src/boards/$(PINOUT)
SRC += $(SIMBA_ROOT)/src/boards/$(PINOUT)/board.c

BOARD_HOMEPAGE = "http://www.kernel.org"
BOARD_PINOUT = "linux-pinout.png"
BOARD_DESC = "Linux"

MCU = linux

GCOV ?= gcov
COVERAGE_REMOVE_FILTER ?=

upload:

rerun:
	@echo "Running $(EXE)"
	(cd $(BUILDDIR) && ./$(notdir $(EXE))) 2>&1 | tee $(RUNLOG) ; \
	test $${PIPESTATUS[0]} -eq 0

run-debugger: all
	gdb $(EXE) --eval-command "break main" --eval-command run

profile:
	gprof $(EXE)

coverage:
	geninfo . -o coverage.info
	if [ ! -z "$(COVERAGE_REMOVE_FILTER)" ] ; then \
	    lcov -r coverage.info $(COVERAGE_REMOVE_FILTER) -o coverage.filtered.info && \
	    mv coverage.filtered.info coverage.info ; \
	fi
	mkdir -p coverage && cd coverage && genhtml ../coverage.info
	@echo
	@echo "Run 'firefox `readlink -f coverage/index.html`' to open the coverage report in a web browser."
	@echo

codecov-coverage:
	geninfo . --gcov-tool $(GCOV) -o coverage.info

jenkins-coverage:
	gcovr -r $(readlink -f ../../..) -x -e ".*main.c"
