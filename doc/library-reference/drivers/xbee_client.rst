:mod:`xbee_client` --- XBee client
==================================

.. module:: xbee_client
   :synopsis: XBee client.

This is a high level driver for XBee modules, providing functions to
execute AT commands, and receive and transmit data from/to a remote
XBee module.

An XBee client consists of an object and a thread. The thread
asynchronously receives frames from the XBee module, leaving this
logic out of the user application. The user can read received packets
(stored in a queue in the client object), transmit data with or
without acknowledgement, and execute AT commands with acknowledgement.

--------------------------------------------------

Source code: :github-blob:`src/drivers/xbee_client.h`, :github-blob:`src/drivers/xbee_client.c`

Example code: :github-blob:`examples/xbee_client/main.c`

--------------------------------------------------

.. doxygenfile:: drivers/xbee_client.h
   :project: simba
