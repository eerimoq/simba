:mod:`usb_device` --- Universal Serial Bus - Device
===================================================

.. module:: usb_device
   :synopsis: Universal Serial Bus - Device.

A USB device is powered and enumerated by a USB host.

The implementation of this module aims to be simple, and yet
flexible. The configuration descriptors are static, allowing them to
be stored in ROM. This saves precious RAM on embedded devices. Class
drivers are initialized and given as input to the device driver before
is is started.

----------------------------------------------

Class drivers:

.. toctree::
   :glob:   

   usb_device/*

----------------------------------------------

Source code: :github-blob:`src/drivers/usb_device.h`

Test code: :github-blob:`tst/drivers/usb_device/main.c`

----------------------------------------------

.. doxygenfile:: drivers/usb_device.h
   :project: simba
