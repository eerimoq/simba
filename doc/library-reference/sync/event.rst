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

----------------------------------------------

Source code: :github-blob:`src/sync/event.h`, :github-blob:`src/sync/event.c`

Test code: :github-blob:`tst/sync/event/main.c`

Test coverage: :codecov:`src/sync/event.c`

----------------------------------------------

.. doxygenfile:: sync/event.h
   :project: simba
