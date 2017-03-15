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

INC += $(SIMBA_ROOT)/src/boards/stm32f3discovery
SRC += $(SIMBA_ROOT)/src/boards/stm32f3discovery/board.c

BOARD_HOMEPAGE = "http://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-discovery-kits/stm32f3discovery.html"
BOARD_PINOUT = "stm32f3discovery.jpg"
BOARD_DESC = "STM32F3DISCOVERY"

MCU = stm32f303vc

upload:
	@echo "Uploading $(NAME).bin"
	st-flash write $(NAME).bin 0x8000000

rerun:
	@echo "Running $(NAME).bin"
	@python -u $(SIMBA_ROOT)/make/run.py \
	     $(RUN_END_PATTERN) \
	     $(RUN_END_PATTERN_SUCCESS) \
	     $(BAUDRATE) | tee run.log
