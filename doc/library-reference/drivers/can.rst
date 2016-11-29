:mod:`can` --- CAN bus
======================

.. module:: can
   :synopsis: CAN bus.

A Controller Area Network (CAN bus) is a vehicle bus standard designed
to allow microcontrollers and devices to communicate with each other
in applications without a host computer. It is a message-based
protocol, designed originally for multiplex electrical wiring within
automobiles, but is also used in many other contexts.

Below is a short example of how to use this module. The error handling
is left out for readability.

.. code-block:: c

   /* Initialize and start the CAN conroller. */
   can_init(&can,
            &can_device[0],
            CAN_SPEED_500KBPS,
            can_rx_buf,
            sizeof(can_rx_buf)) == 0);
   can_start(&can);

   /* Read a frame from the bus. */
   can_read(&can, &frame, sizeof(frame));

   /* Stop the CAN controller. */
   can_stop(&can);

--------------------------------------------------

Source code: :github-blob:`src/drivers/can.h`, :github-blob:`src/drivers/can.c`

Test code: :github-blob:`tst/drivers/can/main.c`

--------------------------------------------------

.. doxygenfile:: drivers/can.h
   :project: simba
