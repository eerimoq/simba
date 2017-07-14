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

Example usage
-------------

This example shows how to receive and tramsmit data from/to a remote
XBee module and control IO on the local XBee module.

See :github-blob:`examples/xbee_client/main.c` for the full example,
including initialization and error handling.

Declaration of the variables used in this example:

.. code-block:: c

   static struct xbee_client_t client;
   ssize_t size;
   uint8_t buf[XBEE_DATA_MAX];
   struct xbee_client_address_t sender;

   /* See full example for initialization. */

Reception and transmission of packets from/to a remote XBee module:

.. code-block:: c

   /* Wait for a packet from a remote XBee module. */
   size = xbee_client_read_from(&client,
                                &buf[0],
                                sizeof(buf),
                                &sender);

   /* Print the read packet and its sender. */
   xbee_client_print_address(sys_get_stdout(), &sender);
   std_hexdump(sys_get_stdout(), &buf[0], size);

   /* Send the packet back to the sender. */
   size = xbee_client_write_to(&client,
                               &buf[0],
                               sizeof(buf),
                               0,
                               &sender);

The classic blink example using a LED connected to the XBee:

.. code-block:: c

   /* Configure DIO0 as output. */
   xbee_client_pin_set_mode(&client,
                            XBEE_PIN_DIO0,
                            XBEE_CLIENT_PIN_OUTPUT);

   /* Blink the LED. */
   while (1) {
      xbee_client_pin_toggle(&client, XBEE_PIN_DIO0);
      thrd_sleep(0.5);
   }

--------------------------------------------------

Source code: :github-blob:`src/drivers/xbee_client.h`, :github-blob:`src/drivers/xbee_client.c`

Example code: :github-blob:`examples/xbee_client/main.c`

--------------------------------------------------

.. doxygenfile:: drivers/xbee_client.h
   :project: simba
