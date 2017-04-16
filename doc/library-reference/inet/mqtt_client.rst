:mod:`mqtt_client` --- MQTT client
==================================

.. module:: mqtt_client
   :synopsis: MQTT client.

MQTT is a publish-subscribe-based lightweight messaging protocol.

.. note:: This driver only implements the MQTT protocol, not the
          transport layer (normally TCP). That has to be set up using
          channels.

Source code: :github-blob:`src/inet/mqtt_client.h`, :github-blob:`src/inet/mqtt_client.c`

Test code: :github-blob:`tst/inet/mqtt_client/main.c`

Test coverage: :codecov:`src/inet/mqtt_client.c`

----------------------------------------------

.. doxygenfile:: inet/mqtt_client.h
   :project: simba
