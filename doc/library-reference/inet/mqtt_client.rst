:mod:`mqtt_client` --- MQTT client
==================================

.. module:: mqtt_client
   :synopsis: MQTT client.

MQTT is a publish-subscribe-based lightweight messaging protocol.

The driver works by running the processing code in a thread which
communicate with the MQTT broker on one side using channels, and the
application on the other side using queues.

This means the application has to set up appropriate channels, which
is already ready to communicate with the MQTT server, e.g. using TCP,
and the thread running the MQTT client.

MQTT Notes
----------

MQTT requires the client to send packets regularly, otherwise the
broker will disconnect the client. Any packet from the client to the
broker will fulfill the requirement, so a regular stream of publish
messages will work or sending MQTT Ping packets as needed. The keep
alive interval is set by the client on connect, and can be changed
from the default in the mqtt_conn_options_t.

.. note:: The current client does not gracefully handle the underlying
          channel (e.g. TCP connection) to the broker disconnecting,
          and requires complete restart of the MQTT client to recover.

Basic MQTT client usage
-----------------------

Basic example of initializing MQTT over TCP (error checking left out
for brevity).

.. code-block:: c

   static size_t on_publish(struct mqtt_client_t *client_p,
                            const char *topic_p,
                            void *chin_p,
                            size_t size)
   {
       uint8_t buf[32];

       chan_read(chin_p, buf, size);
       buf[size] = '\0';
       std_printf(OSTR("on_publish: %s\r\n"), &buf[0]);

       return (0);
   }

.. code-block:: c

   struct inet_addr_t remote_host_address;

   inet_aton("127.0.0.1", &remote_host_address.ip);
   remote_host_address.port = 1883;
   socket_open_tcp(&server_sock);
   socket_connect(&server_sock, &remote_host_address);

   mqtt_client_init(&client,
                    "mqtt_client",
                    NULL,
                    &server_sock,
                    &server_sock,
                    on_publish,
                    NULL);

   thrd_spawn(mqtt_client_main,
              &client,
              0,
              stack,
              sizeof(stack));

   mqtt_client_connect(&client);

Source code: :github-blob:`src/inet/mqtt_client.h`, :github-blob:`src/inet/mqtt_client.c`

Test code: :github-blob:`tst/inet/mqtt_client/main.c`

Test coverage: :codecov:`src/inet/mqtt_client.c`

Example code: :github-blob:`examples/mqtt_client/main.c`

----------------------------------------------

.. doxygenfile:: inet/mqtt_client.h
   :project: simba
