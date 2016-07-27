#
# @file Makefile
# @version 3.1.1
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

all:
	$(MAKE) -C app/console
	$(MAKE) -C app/uds
	$(MAKE) -C app/testapp
	$(MAKE) -C tst/console
	$(MAKE) -C tst/uds

clean:
	$(MAKE) -C app/console clean
	$(MAKE) -C app/uds clean
	$(MAKE) -C app/testapp clean
	$(MAKE) -C tst/console clean
	$(MAKE) -C tst/uds clean
