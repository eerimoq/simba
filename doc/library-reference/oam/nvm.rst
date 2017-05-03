:mod:`nvm` --- Non-volatile memory
==================================

.. module:: nvm
   :synopsis: Non-volatile memory.

A non-volatile memory is typically used for long-term persistent
storage.

This module implements a singleton non-volatile memory, often on top
of an EEPROM or software emulated EEPROM.

----------------------------------------------

Source code: :github-blob:`src/oam/nvm.h`,
:github-blob:`src/oam/nvm.c`

Test coverage: :codecov:`src/oam/nvm.c`

----------------------------------------------

.. doxygenfile:: oam/nvm.h
   :project: simba
