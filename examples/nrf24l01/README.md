ABOUT
-----

Heat/cool the temperature sensor with your fingers to change the color of the RGB LED.

SETUP
-----

Needed devices:

    - 2 x arduino pro mini
    - 1 x ds18b20 temperature sensor
    - 1 x rgb led, or 3 LEDs
    - 3 x ~500 ohm resistors
    - 1 x 4k7 ohm resistor for ds18b20
    - 2 x nrf24l01 wireless transceiver

Connect devices as below.

    +---------+
    | ds18b20 |
    +----o----+
         | OWI
    +----o---+   SPI   +----------+
    |        o---------o          |
    | client |   IRQ   | nrf24l01 |
    |        o---------o          |
    +--------+         +----o-----+
                            |
                           /|\

                                  WIRELESS CONNECTION

                           \|/
                            | 
    +--------+   SPI   +----o-----+
    |        o---------o          |
    | server |   IRQ   | nrf24l01 |
    |        o---------o          |
    +----o---+         +----------+
         |
    +----o----+
    | rgb led |
    +----o----+

BUILD AND RUN
-------------

Build and run the applications.

    >>> make -C server BOARD=arduino_nano release size run

    >>> make -C client BOARD=arduino_nano release size run
