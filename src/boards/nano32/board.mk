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

INC += $(SIMBA_ROOT)/src/boards/nano32
SRC += $(SIMBA_ROOT)/src/boards/nano32/board.c

LINKER_SCRIPT ?= simba.flash.ld
ESP_FLASH_SIZE = 4M

ifeq ($(PARTITION), bootloader)
PARTITIONS_BIN ?= $(SIMBA_ROOT)/3pp/esp32/bin/partitions_bootloader_application.bin
erase: all
upload: erase
else
PARTITIONS_BIN ?= $(SIMBA_ROOT)/3pp/esp32/bin/partitions_singleapp.bin
endif

BOARD_HOMEPAGE = "http://esp32.de"
BOARD_PINOUT = "nano32-pinout.jpg"
BOARD_DESC = "Nano32"

MCU = esp32
SERIAL_PORT ?= /dev/arduino
CONSOLE_RESET_TYPE ?= 0
TIMEOUT ?= 10
BAUDRATE ?= 115200

upload:
	@echo "Uploading $(EXE)."
	python -u $(SIMBA_ROOT)/3pp/esp32/esp-idf/components/esptool_py/esptool/esptool.py \
	--chip esp32 --port $(SERIAL_PORT) --baud 921600 write_flash -z \
	--flash_mode dio --flash_freq 40m --flash_size 4MB \
	0x1000 $(SIMBA_ROOT)/3pp/esp32/bin/bootloader.bin \
	0x10000 $(BIN) \
	0x8000 $(PARTITIONS_BIN)

rerun:
	@echo "Running '$(EXE)'."
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    --timeout $(TIMEOUT) \
			    --baudrate $(BAUDRATE) \
			    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0

erase:
	@echo "Erasing."
	python -u $(ESPTOOL_PY) \
	--chip esp32 --port $(SERIAL_PORT) --baud 921600 erase_flash
