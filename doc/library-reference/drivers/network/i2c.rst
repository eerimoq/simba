:mod:`i2c` --- I2C
==================

.. module:: i2c
   :synopsis: I2C.

I2C is a data transfer bus. Normally one master and one or more slaves
are connected to the bus. The master addresses one slave at a time to
transfer data between the devices.

The master is normally fairly easy to implement since it controls the
bus clock and no race conditions can occur. The slave, on the other
hand, can be implemented in various ways depending on the application
requirements. In this implementation the slave will always send an
acknowledgement when addressed by the master, and lock the bus by
pulling SCL low until it is ready for the transmission.

For systems without hardware I2C, the :doc:`i2c_soft` driver will
supply I2C devices though the interface documented in this driver.

--------------------------------------------------

Source code: :github-blob:`src/drivers/network/i2c.h`, :github-blob:`src/drivers/network/i2c.c`

Test code: :github-blob:`tst/drivers/hardware/network/i2c/master/main.c`

--------------------------------------------------

.. doxygenfile:: drivers/network/i2c.h
   :project: simba
