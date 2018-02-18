#
# @section License
#
# The MIT License (MIT)
#
# Copyright (c) 2018, Erik Moqvist
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

PINOUT ?= xvisor_raspberry_pi_3

INC += $(SIMBA_ROOT)/src/boards/$(PINOUT)
SRC += $(SIMBA_ROOT)/src/boards/$(PINOUT)/board.c

BOARD_HOMEPAGE = "http://xhypervisor.org/"
BOARD_PINOUT = "raspberry-pi-pinout.png"
BOARD_DESC = "Xvisor Raspberry Pi 3"

MCU = xvisor_virt_v8

SERIAL_PORT ?= /dev/arduino
TIMEOUT ?= 10
BAUDRATE ?= 115200

XVISOR_BUILD ?= $(XVISOR_ROOT)/build
XVISOR_DISK ?= $(BUILDDIR)/disk

XVISOR_UVMM_BIN ?= $(XVISOR_BUILD)/uvmm.bin
XVISOR_BOARD_DTS ?= $(SIMBA_ROOT)/src/boards/$(PINOUT)/board.dts
XVISOR_BOARD_DTB ?= $(BUILDDIR)/board.dtb
XVISOR_UDISK_IMG ?= $(BUILDDIR)/udisk.img
XVISOR_UDISK_IMG_SIZE ?= 2048
XVISOR_DTB ?= $(XVISOR_DISK)/images/mcu.dtb
XVISOR_NOR_FLASH_LIST ?= $(SIMBA_ROOT)/src/boards/$(BOARD)/nor_flash.list

CLEAN += $(XVISOR_UVMM_BIN)

XVISOR_PY ?= $(SIMBA_ROOT)/src/boards/$(BOARD)/xvisor.py

TFTP_ROOT ?= /srv/tftp

upload:
	@echo "Copying artifacts to $(TFTP_ROOT)."
	cp $(XVISOR_UVMM_BIN) $(XVISOR_BOARD_DTB) $(XVISOR_UDISK_IMG) $(TFTP_ROOT)
	$(XVISOR_PY) upload

rerun:
	 python3 -u $(XVISOR_PY) rerun \
	    | python3 -u $(BACKTRACE_PY) $(EXE) $(CROSS_COMPILE) \
	    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0

$(XVISOR_UVMM_BIN):
	cd ${XVISOR_ROOT} && \
	export CROSS_COMPILE=aarch64-linux-gnu- && \
	$(MAKE) ARCH=arm generic-v8-defconfig && \
	$(MAKE) && \
	mkimage -A arm64 -O linux -T kernel -C none -a 0x00080000 \
	        -e 0x00080000 -n Xvisor -d $(XVISOR_BUILD)/vmm.bin \
	        $(XVISOR_UVMM_BIN)

$(XVISOR_UDISK_IMG): $(BIN) $(XVISOR_DTS) $(XVISOR_BOARD_DTS)
$(XVISOR_UDISK_IMG): $(XVISOR_NOR_FLASH_LIST) $(XVISOR_UVMM_BIN)
	mkdir -p $(XVISOR_DISK)/{tmp,system,images}
	@echo "Compiling board and mcu device trees..."
	$(XVISOR_BUILD)/tools/dtc/bin/dtc \
	    -I dts \
	    -Wno-unit_address_vs_reg \
	    -p 0x100 \
	    -i $(XVISOR_ROOT)/arch/arm/board/generic/dts/bcm2837 \
	    -O dtb \
	    -o $(XVISOR_BOARD_DTB) \
	    $(XVISOR_BOARD_DTS)
	$(XVISOR_BUILD)/tools/dtc/bin/dtc \
	    -I dts \
	    -O dtb \
	    -o $(XVISOR_DISK)/images/mcu.dtb \
	    $(XVISOR_DTS)
	cp $(XVISOR_NOR_FLASH_LIST) $(XVISOR_DISK)/images/nor_flash.list
	cp $(BIN) $(XVISOR_DISK)/images/simba.bin
	genext2fs -B 1024 -b $(XVISOR_UDISK_IMG_SIZE) -d $(XVISOR_DISK) \
	    $(BUILDDIR)/disk.img
	mkimage -A arm64 -O linux -T ramdisk -a 0x00000000 \
	        -n "Simba Xvisor Ramdisk" -d $(BUILDDIR)/disk.img \
	        $(XVISOR_UDISK_IMG)

build: $(XVISOR_UDISK_IMG)
