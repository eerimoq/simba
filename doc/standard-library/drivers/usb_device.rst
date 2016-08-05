:mod:`usb_device` --- Universal Serial Bus - Device
===================================================

.. module:: usb_device
   :synopsis: Universal Serial Bus - Device.

A USB device is powered and enumerated by a USB host.

The implementation of this module aims to be simple, but yet flexible.
It's possible to change the USB configuration descriptors at runtime
by stopping the current driver, initialize a new driver and start the
new driver. For simple devices only a single configuration is normally
needed.

Using the USB device module is fairly easy. First write the USB
descriptors, then initialize the class drivers, then initialize the
USB device driver and then start it.

See the test code below for an example usage.

----------------------------------------------

Class driver modules:

.. toctree::
   :glob:   

   usb_device/*

----------------------------------------------

Source code: :github-blob:`src/drivers/usb_device.h`, :github-blob:`src/drivers/usb_device.c`

Test code: :github-blob:`tst/drivers/usb_device/main.c`

----------------------------------------------

.. doxygenfile:: drivers/usb_device.h
   :project: simba
