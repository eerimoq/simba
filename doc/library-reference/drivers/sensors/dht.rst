:mod:`dht` --- DHT temperature and humidity sensor
==================================================

.. module:: dht
   :synopsis: DHT temperature and humidity sensor.

DHT is temperature and humidity sensor. Two most
popular versions are DHT11 (blue) and DHT22 (white).

.. image:: ../../../images/drivers/DHT.png
   :width: 50%
   :target: ../../../_images/DHT.png

They are available in bare sensor or module variants.
Sensor has four pins, while module has only three.
According to the specs data pin should be connected to
vcc via 5K pull-up resistor. Notice, that it is not
needed for module variant, since it has already
built-in pull-up resistor.

.. image:: ../../../images/drivers/dht_wiring.png
   :width: 50%
   :target: ../../../_images/dht_wiring.png

Source code: :github-blob:`src/drivers/sensors/dht.h`, :github-blob:`src/drivers/sensors/dht.c`

Example code: :github-blob:`examples/dht/main.c`

----------------------------------------------

.. doxygenfile:: drivers/sensors/dht.h
   :project: simba
