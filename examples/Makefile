#
# @file Makefile
# @version 6.0.0
#
# @section License
# Copyright (C) 2015-2016, Erik Moqvist
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

EXAMPLES = \
	blink \
	bootloader \
	fat16 \
	filesystem \
	hc0x \
	hello_world \
	http_client \
	http_server \
	inet \
	midi_player \
	music_player \
	nrf24l01 \
	owi \
	queue \
	shell \
	spi \
	synthesizer \
	temp \
	timer \
	usb

all: $(EXAMPLES:%=%.all)

clean: $(EXAMPLES:%=%.clean)

$(EXAMPLES:%=%.all):
	$(MAKE) -C $(basename $@) all

$(EXAMPLES:%=%.clean):
	$(MAKE) -C $(basename $@) clean

help:
	@echo "--------------------------------------------------------------------------------"
	@echo "  target                      description"
	@echo "--------------------------------------------------------------------------------"
	@echo "  all                         compile and link the examples"
	@echo
