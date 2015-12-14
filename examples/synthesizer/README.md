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

IMPLEMENTATION DETAILS
======================

The processing chain for a given signal is fairly easy to understand.
First, the note oscillator creates the basic shape of the signal, from
given waveform and vibrato. Then, all active note effects are applied
to the signal. When all notes on a channel have been processed, the
channel effects are applied. Then, as the last step, the stream
effects are applied.

The number of effects to apply is controlled by the user of the
synthesizer in runtime. After all, that's pretty much the purpose of
the synthesizer. =)

OSCILLATORS
-----------

The purpose of the oscillator is to create the basic shape of the
signal. There are three parameters that controls the shape of the
signal; the waveform, the frequency and the vibrato. There are three
kinds of waveforms; square, saw and sine. The frequency and vibrato
are just numerical values.

EFFECTS
-------

Effects can be applied on three levels; note, channel or stream. All
notes on a particular channel applies the same effects.
