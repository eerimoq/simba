:mod:`spiffs` --- SPI Flash File System
=======================================

.. module:: spiffs
   :synopsis: SPI Flash File System.

The source code of this module is based on
https://github.com/pellepl/spiffs.

About
-----

Spiffs is a file system intended for SPI NOR flash devices on embedded
targets.

Spiffs is designed with following characteristics in mind:

- Small (embedded) targets, sparse RAM without heap.
- Only big areas of data (blocks) can be erased.
- An erase will reset all bits in block to ones.
- Writing pulls one to zeroes.
- Zeroes can only be pulled to ones by erase.
- Wear leveling.

---------------------------------------------------

Source code: :github-blob:`src/filesystems/spiffs.h`, :github-blob:`src/filesystems/spiffs.c`

Test code: :github-blob:`tst/filesystems/spiffs/main.c`

---------------------------------------------------

.. doxygenfile:: filesystems/spiffs.h
   :project: simba
