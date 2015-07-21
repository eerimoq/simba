ABOUT
-----

Heat/cool the temperature sensor with your fingers to change the color of the RGB LED.

SETUP
-----

Needed devices:

    - 2 x arduino mini pro
    - 2 x MCP2515 CAN BUS shield
    - 1 x DS18B20 temperature sensor
    - 1 x RGB LED, or 3 LEDs

Connect devices as below.

    +---------+
    | DS18B20 |
    +----o----+
         | OWI
    +----o---+   SPI   +---------+
    |        o---------o         |
    | client |   IRQ   | MCP2515 |
    |        o---------o         |
    +--------+         +--o---o--+
                          |   |
                          |   | CAN BUS
                          |   |
    +--------+   SPI   +--o---o--+
    |        o---------o         |
    | server |   IRQ   | MCP2515 |
    |        o---------o         |
    +----o---+         +---------+
         |
    +----o----+
    | RGB LED |
    +----o----+

BUILD AND RUN
-------------

Build and run the applications.

    >>> make -C server run
    >>> make -C client run
