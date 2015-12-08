ABOUT
=====

A minimalistic synthesizer that creates sound from MIDI and analog
inputs.

See the source code, and mainly main.c, for usage. The documentation
is very limited in this release, and will probably not be improved in
the near future.

Channel 15 is reserved for sound controlled by the potentiometers. POT
0 is used to control the frequency. POT 1 controls the vibrato of all
channels.

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
