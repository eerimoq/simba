:mod:`esp_wifi` --- Espressif WiFi
=======================================

.. module:: esp_wifi
   :synopsis: Espressif WiFi.

This module is a wrapper for the Espressif WiFi interface.

Configure the WiFi as a Station and an Access Point at the same
time. The application tries to connect to a Wifi with SSID `ssid` and
will accept connections to the SSID `Simba`.

.. code-block:: c

   esp_wifi_set_op_mode(esp_wifi_op_mode_station_softap_t);
   esp_wifi_softap_init("Simba", NULL);
   esp_wifi_station_init("ssid", "password", NULL);

Configure the WiFi as an Access Point. The application will accept
connections to the SSID `Simba`.

.. code-block:: c

   esp_wifi_set_op_mode(esp_wifi_op_mode_softap_t);
   esp_wifi_softap_init("Simba", NULL);

Configure the WiFi as a Station. The application tries to connect to a
Wifi with SSID `ssid`.

.. code-block:: c

   esp_wifi_set_op_mode(esp_wifi_op_mode_station_t);
   esp_wifi_station_init("ssid", "password", NULL);

----------------------------------------------

Submodules:

.. toctree::
   :glob:

   esp_wifi/*

----------------------------------------------

Source code: :github-blob:`src/drivers/esp_wifi.h`, :github-blob:`src/drivers/esp_wifi.c`

Test code: :github-blob:`tst/drivers/esp_wifi/main.c`

----------------------------------------------

.. doxygenfile:: drivers/esp_wifi.h
   :project: simba
