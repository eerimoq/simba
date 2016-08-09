.. code-block:: c

   /**
    * @file main.c
    * @version 6.0.0
    *
    * @section License
    * Copyright (C) 2016, Erik Moqvist
    *
    * This library is free software; you can redistribute it and/or
    * modify it under the terms of the GNU Lesser General Public
    * License as published by the Free Software Foundation; either
    * version 2.1 of the License, or (at your option) any later version.
    *
    * This library is distributed in the hope that it will be useful,
    * but WITHOUT ANY WARRANTY; without even the implied warranty of
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    * Lesser General Public License for more details.
    *
    * This file is part of the Simba project.
    */
   
   #include "simba.h"
   
   #if !defined(SSID)
   #    pragma message "WiFi connection variable SSID is not set. Using default value MySSID"
   #    define SSID MySSID
   #endif
   
   #if !defined(PASSWORD)
   #    pragma message "WiFi connection variable PASSWORD is not set. Using default value MyPassword"
   #    define PASSWORD MyPassword
   #endif
   
   /* The ip address of the host to connect to. */
   #define REMOTE_HOST_IP 216.58.211.142
   
   static struct network_interface_wifi_station_espressif_t wifi;
   
   static int init()
   {
       struct inet_ip_addr_t addr;
       char buf[20];
   
       inet_module_init();
       socket_module_init();
       network_interface_module_init();
   
       std_printf(FSTR("Connecting to WiFi with SSID '%s'.\r\n"), STRINGIFY(SSID));
   
       /* Initialize WiFi in station mode with given SSID and
          password. */
       network_interface_wifi_station_espressif_module_init();
       network_interface_wifi_station_espressif_init(&wifi,
                                                     (uint8_t *)STRINGIFY(SSID),
                                                     (uint8_t *)STRINGIFY(PASSWORD));
   
       network_interface_add(&wifi.network_interface);
   
       /* Start WiFi and connect to the Access Point with given SSID and
          password. */
       network_interface_start(&wifi.network_interface);
   
       network_interface_get_ip_address(&wifi.network_interface,
                                        &addr);
   
       std_printf(FSTR("Connected to WiFi with SSID '%s'. Got IP '%s'.\r\n"),
                  STRINGIFY(SSID),
                  inet_ntoa(&addr, buf));
       
       return (0);
   }
   
   int main()
   {
       struct socket_t socket;
       char http_request[] =
           "GET / HTTP/1.1\r\n"
           "Host: " STRINGIFY(REMOTE_HOST_IP) "\r\n"
           "\r\n";
       char http_response[64];
       char remote_host_ip[] = STRINGIFY(REMOTE_HOST_IP);
       struct inet_addr_t remote_host_address;
       
       /* Start the system. Brings up the configured network interfaces
          and starts the TCP/IP-stack. */
       sys_start();
   
       /* Initialize the network stack. This should be moved to the
          network managar when it is implemented. */
       init();
   
       /* Open the tcp socket. */
       socket_open_tcp(&socket);
   
       std_printf(FSTR("Connecting to '%s'.\r\n"), remote_host_ip);
   
       if (inet_aton(remote_host_ip, &remote_host_address.ip) != 0) {
           std_printf(FSTR("Bad ip address ''.\r\n"), remote_host_ip);
           return (-1);
       }
       
       remote_host_address.port = 80;
       
       if (socket_connect(&socket, &remote_host_address) != 0) {
           std_printf(FSTR("Failed to connect to '%s'.\r\n"), remote_host_ip);
           return (-1);
       }
       
       /* Send the HTTP request... */
       if (socket_write(&socket,
                        http_request,
                        strlen(http_request)) != strlen(http_request)) {
           std_printf(FSTR("Failed to send the HTTP request.\r\n"));
           return (-1);
       }
   
       /* ...and receive the first 64 bytes of the response. */
       if (socket_read(&socket,
                       http_response,
                       sizeof(http_response)) != sizeof(http_response)) {
           std_printf(FSTR("Failed to receive the response.\r\n"));
       }
       
       std_printf(FSTR("First 64 bytes of the response:\r\n"
                       "%s"),
                  http_response);
   
       /* Close the socket. */
       socket_close(&socket);
   
       return (0);
   }

