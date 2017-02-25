:mod:`i2c_soft` --- Software I2C
================================

.. module:: i2c_soft
   :synopsis: Software I2C.

I2C is a data transfer bus. Normally one master and one or more slaves
are connected to the bus. The master addresses one slave at a time to
transfer data between the devices.

This driver implements I2C in software for MCUs without I2C hardware
support. For systems with hardware I2C support, the :doc:`i2c` driver
will probably be preferable.

--------------------------------------------------

Source code: :github-blob:`src/drivers/i2c_soft.h`, :github-blob:`src/drivers/i2c_soft.c`

Test code: :github-blob:`tst/drivers/i2c/master_soft/main.c`

--------------------------------------------------

.. doxygenfile:: drivers/i2c_soft.h
   :project: simba
