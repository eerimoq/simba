:mod:`socket` --- Internet communication
========================================

.. module:: socket
   :synopsis: Internet communication.

Source code: :source:`src/inet/inet/socket.h`

Sockets are used to communicate over IP networks. TCP and UDP are the
most common transport protocols.

Below is a TCP client example that connects to a server and sends
data.

.. code-block:: c

   uint8_t buf[16];
   struct socket_t tcp;
   struct socket_addr_t local_addr, remote_addr;

   /* Set the local and remote addresses. */   
   local_addr.ip = 0x6801a8c0;
   local_addr.port = 6000;
   remote_addr.ip = 0x6a01a8c0;
   remote_addr.port = 5000;

   /* Initialize the socket and connect to the server. */
   socket_init(&tcp, SOCKET_DOMAIN_AF_INET, SOCKET_TYPE_STREAM);
   socket_bind(&tcp, &local_addr, sizeof(local_addr));
   socket_connect(&tcp, &remote_addr, sizeof(remote_addr));

   /* Send the data. */
   memset(buf, 0, sizeof(buf));
   socket_write(&tcp, buf, sizeof(buf));

   /* Close the connection. */
   socket_close(&tcp);

And below is the same scenario for UDP.

.. code-block:: c

   uint8_t buf[16];
   struct socket_t udp;
   struct socket_addr_t local_addr, remote_addr;

   /* Set the local and remote addresses. */   
   local_addr.ip = 0x6801a8c0;
   local_addr.port = 6000;
   remote_addr.ip = 0x6a01a8c0;
   remote_addr.port = 5000;

   /* Initialize the socket and connect to the server. */
   socket_init(&udp, SOCKET_DOMAIN_AF_INET, SOCKET_TYPE_STREAM);
   socket_bind(&udp, &local_addr, sizeof(local_addr));
   socket_connect(&udp, &remote_addr, sizeof(remote_addr));

   /* Send the data. */
   memset(buf, 0, sizeof(buf));
   socket_send(&udp, buf, sizeof(buf));

   /* Close the connection. */
   socket_close(&udp);
 
----------------------------------------------

.. doxygenfile:: inet/socket.h
   :project: simba
