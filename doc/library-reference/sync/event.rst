:mod:`event` --- Event channel
==============================

.. module:: event
   :synopsis: Event channel.

An event channel consists of a 32 bits bitmap, where each bit
corresponds to an event state. If the bit is set, the event is
active. Since an event only has two states, active and inactive,
signalling the same event multiple times will just result in the event
to be active. There is no internal counter of how "active" an event
is, it's simply active or inactive.

Example usage
-------------

This is a small example of writing an event from an interrupt handler
to a thread.

.. code-block:: c

   struct event_t event;

   /* The interrupt handler. */
   ISR(foo)
   {
       uint32_t mask;

       mask = 0x1;
       event_write_isr(&event, &mask, sizeof(mask));
   }

   /* The thread. */
   void bar(void *arg_p)
   {
       uint32_t mask;

       /* Must be called before any read from or write to the event
          channel. */
       event_init(&event);

       mask = 0x1;
       event_read(&event, &mask, sizeof(mask))

       /* Do something with the event. */
   }

----------------------------------------------

Source code: :github-blob:`src/sync/event.h`, :github-blob:`src/sync/event.c`

Test code: :github-blob:`tst/sync/event/main.c`

Test coverage: :codecov:`src/sync/event.c`

----------------------------------------------

.. doxygenfile:: sync/event.h
   :project: simba
