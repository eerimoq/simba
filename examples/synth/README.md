ABOUT
=====

A minimalistic synthesizer that creates audio from MIDI and analog
inputs.

HARDWARE
========

Needed devices:

    - 1 x Arduino Due
    - 2 x 10k linear potentiometers (POT)
    - 2 x 10k ohm resistors
    - cables

For MIDI input (optional):
    - 1 x Female DIN-5 (MIDI connector)
    - 1 x 220 ohm resistor
    - 1 x 4k7 ohm resistor (pull-up)

SCHEMATICS
----------

      ,-----.      ,-----.     ,-----.
     /       \    /       \   /       \
    |  POT 0  |  |  POT 1  | |  DIN-5  |
     \       /    \       /   \ 2 5 4 /
      `o-o-o´      `o-o-o´     `o-o-o´
       | | |        | | |       | | |
       | | |        | | |       | |220
       | | |        | | |       | | |
       | | |        | | |       | +----4k7-----+
       | | |        | | |       | | |          |
       | | +--------|-|-+-----------+----------+
       +-|----------+-|---------+------------+ |
         |            |           |          | |
    +----o------------o-----------o-------+  | |
    |    a0           a1         rx1      |  | |
    |                                 3v3 o--|-+
    |            Arduino Due              |  |
    |                                     |  |
    |                                 GND o--+
    |          dac0        dac1           |  |
    +-----------o-----------o-------------+  |
                |           |                |
               10k         10k               |
                |           |                |
                |    +------+     +----------+
                |    |           /
                |    |          /    ---------------
              .-|-+--|-+-------|----|XXXXXXXXXXXXXXX
             (  x |  x |       x    |XXX 3.5 mm  XXX -- AMPLIFIER & SPEAKER
              `---+----+------------|XXXXXXXXXXXXXXX
                                     ---------------

BUILD AND RUN
=============

Build and run the applications.

    >>> make BOARD=arduino_due release size run
