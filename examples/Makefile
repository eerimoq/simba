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

EXAMPLES = \
	analog_read \
	analog_write \
	blink \
	upgrade/application \
	upgrade/bootloader \
	ds18b20 \
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
	ssl_client \
	ssl_server \
	synthesizer \
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
