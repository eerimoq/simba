:mod:`eeprom_i2c` --- I2C EEPROM
================================

.. module:: eeprom_i2c
   :synopsis: I2C EEPROM.

Below is a list of I2C EEPROMs that are known to work with this
driver. Other I2C EEPROMs may work as well, as they often implement
the same interface.

- AT24C32 from Atmel.
- AT24C256 from Atmel.

Known limitations:

- Only supports 16 bits addressing. 8 bits addressing can easily be
  added.

----------------------------------------------
  
Source code: :github-blob:`src/drivers/storage/eeprom_i2c.h`,
:github-blob:`src/drivers/storage/eeprom_i2c.c`

Test code: :github-blob:`tst/drivers/hardware/eeprom_i2c/main.c`

----------------------------------------------

.. doxygenfile:: drivers/storage/eeprom_i2c.h
   :project: simba
