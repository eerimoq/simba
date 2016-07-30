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

Connect devices as below.

    +--------------------+
    | GND  ds18b20   VCC |
    +--o-------o------o--+
       |       |      |
       |       +--4k7-+     OWI
       |       |      |
    +--o-------o------o--+
    | GND     d7     VCC |
    |       client       |
    |  d6  d11  d12  d13 |
    +--o----o----o----o--+
       |    |    |    |
       |    |    |    |     SPI
       |    |    |    |
    +--o----o----o----o--+
    | d10  d11  d12  d13 |
    |       server       |
    | d7   d8   d9   GND |
    +--o----o----o----o--+
       |    |    |    |
      .5k  .5k  .5k   |
       |    |    |    |
    +--o----o----o--+ |
    |    rgb led    | |
    +-------o-------+ |
            |         |
            +---------+

BUILD AND RUN
-------------

Build and run the applications.

    >>> make -C server BOARD=arduino_nano release size run

    >>> make -C client BOARD=arduino_nano release size run
