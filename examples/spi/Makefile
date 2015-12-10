#
# @file Makefile
# @version 1.0
#
# @section License
# Copyright (C) 2015, Erik Moqvist
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

APPS = client server

all: $(APPS:%=%.all)

clean: $(APPS:%=%.clean)

$(APPS:%=%.all):
	$(MAKE) -C $(basename $@) all

$(APPS:%=%.clean):
	$(MAKE) -C $(basename $@) clean

help:
	@echo "--------------------------------------------------------------------------------"
	@echo "  target                      description"
	@echo "--------------------------------------------------------------------------------"
	@echo "  all                         compile and link the demos"
	@echo
